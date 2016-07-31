/**
 * @brief 简单文件系统，参考http://kukuruku.co/hub/nix/writing-a-file-system-in-linux-kernel
 *
 * 有问题，rmmod会崩溃
 * 测试：
 *  touch image
 *  mkdir dir
 *  sudo mount -o loop -t aufs ./image ./dir
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>

#define AUFS_MAGIC_NUMBER 0x12345678

static void aufs_put_super(struct super_block *sb) {
    pr_debug("aufs super block destroyed\n");
}

static struct super_operations const aufs_super_ops = {
    .put_super = aufs_put_super,
};

static int aufs_fill_sb(struct super_block *sb, void *data, int silent) {
    struct inode *root = NULL;

    sb->s_magic = AUFS_MAGIC_NUMBER;
    sb->s_op = &aufs_super_ops;

    root = new_inode(sb);
    if (!root) {
        pr_err("inode allocation failed\n");
        return -ENOMEM;
    }

    root->i_ino = 0;
    root->i_sb = sb;
    root->i_atime = root->i_mtime = root->i_ctime = CURRENT_TIME;
    inode_init_owner(root, NULL, S_IFDIR);

    sb->s_root = d_make_root(root);
    if (!sb->s_root) {
        pr_err("root creation failed\n");
        return -ENOMEM;
    }
    return 0;
}

static struct dentry *aufs_mount(struct file_system_type *type, int flags,
                char const *dev, void *data) {
    struct dentry *entry = mount_bdev(type, flags, dev, data, aufs_fill_sb);
    if (IS_ERR(entry)) {
        pr_err("aufs mounting failed\n");
    } else {
        pr_err("aufs mounted\n");
    }
    return entry;
}

static struct file_system_type aufs_type = {
    .owner = THIS_MODULE,
    .name = "aufs",
    .mount = aufs_mount,
    .kill_sb = kill_block_super,
    .fs_flags = FS_REQUIRES_DEV,
};

static int __init aufs_init(void) {
    register_filesystem(&aufs_type);
    pr_debug("aufs module loaded\n");
    return 0;
}

static void __exit aufs_exit(void) {
    pr_debug("aufs module unloaded\n");
}

module_init(aufs_init);
module_exit(aufs_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a simple module");

