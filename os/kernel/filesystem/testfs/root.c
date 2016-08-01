#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

#define TESTFS_MAGIC 0x12344321

static int testfs_fill_super(struct super_block *sb, void *data, int silent)
{
    static struct tree_descr debug_files[] = {{""}};
    pr_debug("testfs fill_super\n");
    simple_fill_super(sb, TESTFS_MAGIC, debug_files);
    return 0;
}

static struct dentry *testfs_mount(struct file_system_type *fs_type,
    int flags, const char *dev_name, void *data)
{
    pr_debug("testfs mount\n");
    return mount_nodev(fs_type, flags, data, testfs_fill_super);
}

static void testfs_kill_sb(struct super_block *sb)
{
    pr_debug("testfs kill sb\n");
    kill_litter_super(sb);
}

static struct file_system_type testfs_type = {
    .owner = THIS_MODULE,
    .name = "testfs", /* 文件类型名称 */
    .mount = testfs_mount,
    /* 查看/proc/filesystems内容为"nodev   testfs" */
    .kill_sb = testfs_kill_sb,
    .fs_flags = FS_USERNS_MOUNT,

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

