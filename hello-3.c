/*
* hello3.c - shows __init, __initdata and __exit macros.
* __init causes the init function to be discarded and the memory used by it to be 
*        freed when the module it's builtin.
* __exit causes the exit function to be discarded when the module it's builtin.
* __initdata it's simila to __init but for variables instead of functions.
*/

#include <linux/module.h> 	// needed by all modules
#include <linux/kernel.h>	// needed for KERN_INFO
#include <linux/init.h>		// needed for the macros

static int hello3_data __initdata=3;

static int __init hello_3_init(void){
	printk(KERN_INFO "Hello world %d.\n", hello3_data);
	return 0;
}

static void __exit hello_3_exit(void){
	printk(KERN_INFO "Goodbye world 3.\n");
}

module_init(hello_3_init);
module_exit(hello_3_exit);
