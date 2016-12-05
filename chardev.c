/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */

/*
 *  Prototypes - this would normally go in a .h file
 */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0 		/* all is fine  */
#define DEVICE_NAME "chardev" 	/* name of the device file */
#define BUF_LEN 80 		/* message max length */

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open? just one device read at a time */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;		/* Pointer to the message */

/*
 * Supported file_operations implementations.
 * .write throws an error, we're read only.
 */
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * Module initialization and cleanup.
 */

/*
 * This function is called when the module is loaded
 */
int init_module(void){

	// Registering the device with a major version of 0 means we want get a
	// major version from the system.
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if(Major < 0){
		// Let's log the error code.
		printk(KERN_ALERT "chardev: Registering char device failed with %d\n", Major);
		// And return it.
		return Major;
	}

	// printing some info on how to use this module.

	printk(KERN_INFO "chardev: I was assigned major number %d.\n", Major);
	printk(KERN_INFO "chardev: To talk to the driver, create a dev file with\n");
	printk(KERN_INFO "chardev: 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "chardev: Try various minor numbers.\n");
	printk(KERN_INFO "chardev: Try to cat and echo to the device file.\n");
	printk(KERN_INFO "chardev: Remove the device file and module when done.\n");

	return SUCCESS;
}


/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void){

	printk(KERN_INFO "chardev: unregistered, thank you.\n");
	// Unregistering the device.
	// Used to return an int, always succeedes now!
	unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * file_operations method implementations.
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file){

	static int counter = 0;

	// Only one process can open the driver at a given time.
	if(Device_Open){
		return -EBUSY;
	}

	Device_Open++;
	sprintf(msg, "chardev: I already told you %d times Hello world!\n", counter++);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file){

	// We're now ready for our next caller
	Device_Open--;

	/*
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module.
	 */
	module_put(THIS_MODULE);

	return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset){
	int bytes_read = 0;

	// If no msg to return.
	if(*msg_Ptr == 0){
		// Return 0 to tell we're done.
		return SUCCESS;
	}

	// Actually put the data into the buffer
	while (length && *msg_Ptr) {

		/*
		 * The buffer is in the user data segment, not the kernel
		 * segment so "*" assignment won't work.  We have to use
		 * put_user which copies data from the kernel data segment to
		 * the user data segment.
		 */
		put_user(*(msg_Ptr++), buffer++);

		length--;
		bytes_read++;
	}

	// We return the number of bytes read.
	return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off){
	printk(KERN_ALERT "chardev: Sorry, this operation isn't supported.\n");
	return -EINVAL;
}
