#include "common.h"


int ioctl_device_num;
struct class *ioctl_device_class = NULL;
struct device *ioctl_device = NULL;

struct file_operations ioctl_ops =
{
    // .read = device_read,
    // .write = device_write,
    // .open = device_open,
    // .release = device_close,
    .unlocked_ioctl = test_ioctl,
};

/*
在定义函数命令的时候，使用2往往没有任何效果
*/

long test_ioctl(struct file* filep, unsigned int cmd, unsigned long arg){
    switch (cmd)
    {
    case TEST_IOCTL_ZERO:
        printk(KERN_INFO "LEHR %s cmd Zero %d\n", __func__, cmd);
        break;

    case TEST_IOCTL_FIRST:
        printk(KERN_INFO "LEHR %s cmd Fisrt %d\n", __func__, cmd);
        break;
    
    case TEST_IOCTL_SECOND:
        printk(KERN_INFO "LEHR %s cmd Second %d\n", __func__, cmd);
        break;

    default:
        printk(KERN_INFO "LEHR %s cmd default %d\n", __func__, cmd);
        break;
    }
    return 1ll;
}

int ioctl_device_init(void){
    /*@第一个参数如果大于0则表示由系统分配设备主序号，从序号一般在这里看不出来
    *@第二个参数是device的名称，如果没有指明根目录，一般会创建在/dev下
    *@第三个是文件操作结构体
    *@如果没有知名主序号，那么返回值就是主序号major
    */
    ioctl_device_num = register_chrdev(0, IOCTL_DEVICE_NAME, &ioctl_ops);

    if(ioctl_device_num < 0){
        pr_err("LEHR_IOCTL register ioctl device fail!\n");
        return -1;
    }

    /*device 的class
    */
    ioctl_device_class = class_create(THIS_MODULE, IOCTL_DEVICE_CLASS_NAME);
    if(ioctl_device_class == NULL){
        //解注册的时候传入的是主序号名称
        unregister_chrdev(ioctl_device_num, IOCTL_DEVICE_NAME);
        pr_err("LEHR_IOCTL create ioctl device class fail!\n");
        return -1;
    }

    /*
    MKDEV(major, minor)，第一个参数是主序号，第二个参数是从序号
    */
    ioctl_device = device_create(ioctl_device_class, NULL, MKDEV(ioctl_device_num, 0), NULL, IOCTL_DEVICE_NAME);
    if(ioctl_device == NULL){
        class_destroy(ioctl_device_class);
        unregister_chrdev(ioctl_device_num, IOCTL_DEVICE_NAME);
        pr_err("LEHR_IOCTL create ioctl device fail!\n");
        return -1;
    }

    printk(KERN_INFO "LEHR_IOCTL ioctl device register succeed!\n");
    return 1;
}

void ioctl_device_exit(void){
    device_destroy(ioctl_device_class, MKDEV(ioctl_device_num, 0));
    class_destroy(ioctl_device_class);
    unregister_chrdev(ioctl_device_num, IOCTL_DEVICE_NAME);
    printk(KERN_INFO "LEHR_IOCTL exit!\n");
}
