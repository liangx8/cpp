#include <linux/init.h>
#include <linux/module.h>

static int __exit hello_init(void)
{
	printk(KERN_ALERT "Hello linux driver module\n");
	return 0;
}
static void __init hello_exit(void)
{
	printk(KERN_ALERT "Goodbye linux driver module\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("nobody!");
