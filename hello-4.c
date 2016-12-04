/*  
 *  hello-4.c - Demonstrates module documentation.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

#define DRIVER_AUTHOR "Mauro Scomparin <scompo@gmail.com>"
#define DRIVER_DESC   "A sample driver"

static int __init init_hello_4(void)
{
	printk(KERN_INFO "Hello, world 4.\n");
	return 0;
}

static void __exit cleanup_hello_4(void)
{
	printk(KERN_INFO "Goodbye, world 4.\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

// This is how you use a string for the documentation macros.
MODULE_LICENSE("GPL");

// This is how to use defines for the documentation macros.
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

/*
 * This says my module uses /dev/testdevice.
 * The MODULE_SUPPORTED_DEVICE macro can be used also for
 * automatic configuration.
 * Right now it's used for documentation purposes.
 */
MODULE_SUPPORTED_DEVICE("testdevice");
