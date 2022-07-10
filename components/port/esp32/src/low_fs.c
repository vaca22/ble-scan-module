
#include "stdlib.h"
#include "array.h"

#include "ff.h"
#include "plat_os.h"
#include "low_fs.h"

typedef struct
{
    int32_t id;
    union
    {
        FIL *fp;
        DIR *dp;
    };
} low_fs_t;

static char *fs_rev_name(int32_t res);

int32_t fs_id = 0;
Array *fs_ar = NULL;
static os_semaphore_t sem;

low_fs_t *low_fs_obj_find_by_id(int32_t id)
{
    os_semaphore_get(&sem, OS_WAIT_FOREVER);
    ARRAY_FOREACH(tmp, fs_ar,
    {
        low_fs_t *fs = (low_fs_t *)tmp;
        if(fs->id == id)
        {
            os_semaphore_put(&sem);
            return fs;
        }
    }
    );
    os_semaphore_put(&sem);
    return NULL;
}

low_fs_t *low_fs_obj_create(void)
{
    low_fs_t *fs = os_mem_alloc(sizeof(*fs));
    fs->id = fs_id++;
    array_add(fs_ar, fs);
    return fs;
}

void low_fs_obj_add(low_fs_t *fs)
{
    os_semaphore_get(&sem, OS_WAIT_FOREVER);
    fs->id = fs_id++;
    array_add(fs_ar, fs);
    os_semaphore_put(&sem);
}

void low_fs_obj_delete(low_fs_t *fs)
{
    os_semaphore_get(&sem, OS_WAIT_FOREVER);
    array_remove(fs_ar, fs, NULL);
    os_mem_free(fs->fp);
    os_mem_free(fs);
    os_semaphore_put(&sem);
}

void low_fs_init(void)
{
    array_new(&fs_ar);
    os_semaphore_create(&sem, "low_fs", 1);
}

int32_t low_fs_path_exist(char *path)
{
    int32_t rev;
    FILINFO *fno = os_mem_alloc(sizeof(*fno));
    rev = f_stat(path, fno);
    os_mem_free(fno);
    return rev == FR_OK;
}

int32_t low_fs_path_delete(char *path)
{
    //f_unlink(path);
    int32_t rev = f_unlink(path);
    printf("[low_fs]delete %s rev:%s\n", path, fs_rev_name(rev));
    return 0;
}

int32_t low_fs_folder_create(char *path)
{
    int32_t rev;
    if(low_fs_path_exist(path))
    { //如果目录已经存在了，就什么也不做
        //printf("[low_fs]folder_create:%s already exist\n", path);
        return 0;
    }
    rev = f_mkdir(path);
    if(rev != FR_OK && rev != FR_EXIST)
    {
        printf("[low_fs]folder_create:%s failed:%s\n", path, fs_rev_name(rev));
        return -1;
    }
    return 0;
}

int32_t low_fs_open(char *path, int32_t flag)
{
    int32_t rev; 
    FIL *fp;
    if(flag & FS_O_CREAT || flag & FS_O_APPEND || flag & FS_O_TRUNC)
    { //是创建文件，这里检查目录是否存在
        char *end, *path_folder;
        path_folder= strdup(path);
        end = strrchr(path_folder, '/');
        if(end == NULL)
        {
            printf("[low_fs]open err path:%s\n", path);
            free(path_folder);
            return -1;
        }
        if(end != path)
        {
            *end = 0;
            low_fs_folder_create(path_folder);
            free(path_folder);
        }
        else
        {//根目录,不用创建
            printf("[low_fs]open root path\n");
        }
    }

    fp = os_mem_alloc(sizeof(*fp));
    rev = f_open(fp, path, flag);
    if(rev == FR_OK)
    {
        low_fs_t *fs = os_mem_alloc(sizeof(*fs));
        fs->fp = fp;
        low_fs_obj_add(fs);
        //printf("[low_fs]open:%s ok\n", path);
        return fs->id;
    }
    os_mem_free(fp);
    printf("[low_fs]open:%s err:%s\n", path, fs_rev_name(rev));
    return -1;
}

int32_t low_fs_close(int32_t fd)
{
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    if(fs == NULL)
    {
        printf("[low_fs]close err id:%d\n", fd);
        return -1;
    }
    f_close(fs->fp);
    low_fs_obj_delete(fs);
    return 0;
}

int64_t low_fs_read(int32_t fd, void *buf, uint64_t size)
{
    int32_t rev;
    uint32_t rb = 0;
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    rev = f_read(fs->fp, buf, size, &rb);
    if(rev == FR_OK)
    {
        return rb;
    }
    printf("[low_fs]read failed:%s rb:%d\n", fs_rev_name(rev), rb);
    return -1;
}

int64_t low_fs_write(int32_t fd, void *buf, uint64_t size)
{
    int32_t rev;
    uint32_t wb = 0;
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    rev = f_write(fs->fp, buf, size, &wb);
    if(rev == FR_OK)
    {
        return wb;
    }
    printf("[low_fs]write failed:%s wb:%d\n", fs_rev_name(rev), wb);
    return -1;
}

uint64_t low_fs_seek(int32_t fd, int64_t offset, int32_t flag)
{
    int32_t rev;
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    if(flag == FS_SEEK_SET)
    {
        //rev = f_lseek(fs->fp, offset);
    }
    else if(flag == FS_SEEK_CUR)
    {
        offset += f_tell(fs->fp);
    }
    else
    {
        offset += f_size(fs->fp);
    }
    rev = f_lseek(fs->fp, offset);
    if(rev == FR_OK)
    {
        return offset;
    }
    printf("[low_fs]seek failed:%s\n", fs_rev_name(rev));
    return -1;
}

int32_t low_fs_path_isdir(char *path)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_path_isfile(char *path)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int64_t low_fs_file_size(char *path)
{
    int32_t rev;
    int64_t size = -1;
    FILINFO *fno = os_mem_alloc(sizeof(*fno));
    rev = f_stat(path, fno);
    if(rev == FR_OK)
    {
        size = fno->fsize;
    }
    os_mem_free(fno);
    return size;
}

int64_t low_fs_dir_size(char *path)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_file_copy(char *src, char *dst)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_file_move(char *src, char *dst)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_file_time_get(char *path)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_file_data_set(char *path, void *data, int32_t size)
{
    int32_t rev, fd = low_fs_open(path, FS_O_WRITE | FS_O_TRUNC);
    if(fd < 0)
    {
        printf("[low_fs]data_set open:%s failed\n", path);
        return -1;
    }
    rev = low_fs_write(fd, data, size);
    low_fs_close(fd);
    if(rev == size)
    {
        return 0;
    }
    printf("[low_fs]data_set size:%d but write:%d\n", size, rev);
    return -1;
}

int32_t low_fs_file_data_append(char *path, void *data, int32_t size)
{
    int32_t rev, fd = low_fs_open(path, FS_O_WRITE | FS_O_APPEND);
    if(fd < 0)
    {
        printf("[low_fs]data_append open:%s failed\n", path);
        return -1;
    }
    rev = low_fs_write(fd, data, size);
    low_fs_close(fd);
    if(rev == size)
    {
        return 0;
    }
    printf("[low_fs]data_append size:%d but write:%d\n", size, rev);
    return -1;
}

int32_t low_fs_file_data_get(char *path, void *data, int32_t *size)
{
    FIL *fp = os_mem_alloc(sizeof(*fp));
    int32_t rev, file_size, open_flag = 0;
    uint32_t rb;
    uint8_t *buf = NULL;
    rev = f_open(fp, path, FS_O_READ);
    if(rev != FR_OK)
    {
        printf("[low_fs]data_get path:%s failed:%s\n", path, fs_rev_name(rev));
        os_mem_free(fp);
        return -1;
    }
    open_flag = 1;
    file_size = f_size(fp);
    buf = os_mem_alloc(file_size);
    if(buf == NULL)
    {
        printf("[low_fs]data_get alloc size:%d failed\n", file_size);
        rev = -1;
        goto low_fs_file_data_get_exit;
    }
    rb = 0;
    rev = f_read(fp, buf, file_size, &rb);
    if(rev != FR_OK)
    {
        printf("[low_fs]data_get read failed:%s\n", fs_rev_name(rev));
        os_mem_free(buf);
        buf = NULL;
        rev = -1;
    }
    else
    {
        *size = rb;
    }
low_fs_file_data_get_exit:
    if(open_flag)
    {
        f_close(fp);
    }
    *(uint8_t **)data = buf;
    os_mem_free(fp);
    return rev;
}

int32_t low_fs_file_data_get_free(void *data)
{
    if(data)
    {
        os_mem_free(data);
    }
    return 0;
}

void *low_fs_dir_open(char *path)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_dir_close(void *dir)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_dir_read(void *dir, char *child_name)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_fsync(int32_t fd)
{
    int32_t rev;
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    rev = f_sync(fs->fp);
    if(rev == FR_OK)
    {
        return 0;
    }
    printf("[low_fs]fsync failed:%s\n", fs_rev_name(rev));
    return -1;
}

int32_t low_fs_fallocate(int32_t fd, int32_t mode, int64_t offset, int64_t len)
{
    printf("[low_fs]%s empty!!\n", __func__);
    return 0;
}

int32_t low_fs_ftruncate(int32_t fd, int64_t size)
{
    int32_t rev;
    low_fs_t *fs = low_fs_obj_find_by_id(fd);
    rev = f_lseek(fs->fp, size);
    if(rev != FR_OK)
    {
        printf("[low_fs]ftruncate err size:%lld\n", size);
        return -1;
    }
    rev = f_truncate(fs->fp);
    if(rev != FR_OK)
    {
        printf("[low_fs]ftruncate failed:%s\n", fs_rev_name(rev));
        return -1;
    }
    return 0;
}

static char *fs_rev_name(int32_t res)
{
const char *res_buf[] =
    {
	"FR_OK",				    /* (0) Succeeded */
	"FR_DISK_ERR",			    /* (1) A hard error occurred in the low level disk I/O layer */
	"FR_INT_ERR",				/* (2) Assertion failed */
	"FR_NOT_READY",			    /* (3) The physical drive cannot work */
	"FR_NO_FILE",				/* (4) Could not find the file */
	"FR_NO_PATH",				/* (5) Could not find the path */
	"FR_INVALID_NAME",		    /* (6) The path name format is invalid */
	"FR_DENIED",				/* (7) Access denied due to prohibited access or directory full */
	"FR_EXIST",				    /* (8) Access denied due to prohibited access */
	"FR_INVALID_OBJECT",		/* (9) The file/directory object is invalid */
	"FR_WRITE_PROTECTED",		/* (10) The physical drive is write protected */
	"FR_INVALID_DRIVE",		    /* (11) The logical drive number is invalid */
	"FR_NOT_ENABLED",			/* (12) The volume has no work area */
	"FR_NO_FILESYSTEM",		    /* (13) There is no valid FAT volume */
	"FR_MKFS_ABORTED",		    /* (14) The f_mkfs() aborted due to any problem */
	"FR_TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
	"FR_LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
	"FR_NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
	"FR_TOO_MANY_OPEN_FILES",	/* (18) Number of open files > FF_FS_LOCK */
	"FR_INVALID_PARAMETER"	    /* (19) Given parameter is invalid */
    };
    if(res > FR_INVALID_PARAMETER)
    {
        return "ERR_RESULT";
    }
    return (char *)res_buf[res];
}
