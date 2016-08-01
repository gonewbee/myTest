#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

static struct dentry *testfs_mount(struct file_system_type *fs_type,
    int flags, const char *dev_name, void *data)
{
    pr_debug("testfs mount\n");
    return NULL;
}

static void testfs_kill_sb(struct super_block *sb)
{
    pr_debug("testfs kill sb\n");
}

static struct file_system_type testfs_type = {
    .owner = THIS_MODULE,
    .name = "testfs", /* 文件类型名称 */
    .mount = testfs_mount,
    /* 查看/proc/filesystems内容为"nodev   testfs" */
    .kill_sb = testfs_kill_sb,
    .fs_flags = FS_USERNS_VISIBLE | FS_USERNS_MOUNT,

    /* 查看/proc/filesystems内容为"        testfs" */
    // .kill_sb = kill_block_super,
    // .fs_flags = FS_REQUIRES_DEV,
};

static int __init testfs_init(void)
{
    int err;
    pr_debug("testfs module loaded\n");
    /* 加载驱动后查看/proc/filesystems可以看到testfs */
    err = register_filesystem(&testfs_type);
    if (err) {
        pr_err("testfs register_filesystem error\n");
    }
    return 0;
}

static void __exit testfs_exit(void)
{
    int err;
    pr_debug("testfs module unloaded\n");
    err = unregister_filesystem(&testfs_type);
    if (err) {
        pr_err("testfs unregister_filesystem error\n");
    }
}

module_init(testfs_init);
module_exit(testfs_exit);

MODULE_LICENSE("GPL");

