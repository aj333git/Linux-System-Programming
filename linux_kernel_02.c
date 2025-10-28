#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "my_value"
#define BUFFER_SIZE 128

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amit Joshi");
MODULE_DESCRIPTION("A simple /proc entry example");
MODULE_VERSION("0.1");

static struct proc_dir_entry *proc_entry;
static char proc_buffer[BUFFER_SIZE];
static int proc_value = 0;

// Called when /proc/my_value is read
ssize_t proc_read(struct file *file, char __user *user_buffer,
                  size_t count, loff_t *pos) {
    int len = snprintf(proc_buffer, BUFFER_SIZE, "%d\n", proc_value);
    if (*pos > 0 || count < len)
        return 0;
    if (copy_to_user(user_buffer, proc_buffer, len))
        return -EFAULT;
    *pos = len;
    return len;
}

// Called when /proc/my_value is written
ssize_t proc_write(struct file *file, const char __user *user_buffer,
                   size_t count, loff_t *pos) {
    char buffer[BUFFER_SIZE];
    if (count > BUFFER_SIZE - 1)
        return -EFAULT;
    if (copy_from_user(buffer, user_buffer, count))
        return -EFAULT;
    buffer[count] = '\0';
    sscanf(buffer, "%d", &proc_value);
    printk(KERN_INFO "proc_example: new value = %d\n", proc_value);
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

// Module init
static int __init proc_example_init(void) {
    proc_entry = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        printk(KERN_ERR "proc_example: Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "proc_example: /proc/%s created\n", PROC_NAME);
    return 0;
}

// Module exit
static void __exit proc_example_exit(void) {
    proc_remove(proc_entry);
    printk(KERN_INFO "proc_example: /proc/%s removed\n", PROC_NAME);
}

module_init(proc_example_init);
module_exit(proc_example_exit);

