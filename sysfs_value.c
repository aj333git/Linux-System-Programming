#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amit Joshi");
MODULE_DESCRIPTION("Simple sysfs example");
MODULE_VERSION("0.1");

static struct kobject *example_kobj;
static int sys_value = 0;

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", sys_value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count) {
    sscanf(buf, "%d", &sys_value);
    printk(KERN_INFO "sysfs_example: new value = %d\n", sys_value);
    return count;
}

static struct kobj_attribute value_attribute = __ATTR(value, 0660, value_show, value_store);

static int __init sysfs_example_init(void) {
    example_kobj = kobject_create_and_add("sysfs_value", kernel_kobj);
    if (!example_kobj)
        return -ENOMEM;

    if (sysfs_create_file(example_kobj, &value_attribute.attr)) {
        kobject_put(example_kobj);
        return -EFAULT;
    }

    printk(KERN_INFO "sysfs_example: Module loaded\n");
    return 0;
}

static void __exit sysfs_example_exit(void) {
    kobject_put(example_kobj);
    printk(KERN_INFO "sysfs_example: Module unloaded\n");
}

module_init(sysfs_example_init);
module_exit(sysfs_example_exit);

