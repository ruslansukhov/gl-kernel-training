#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/time.h>


#define LEN_MSG 160
static char* buf_msg;
static struct timeval* tv;


static ssize_t xxx_show( struct class *class, struct class_attribute *attr, char *buf ) {
	strcpy( buf, buf_msg );
	printk( "previous absolute read time: %ld s %ld mks\n", tv->tv_sec, tv->tv_usec );
	printk( "read %ld\n", (long)strlen( buf ) );
	do_gettimeofday(tv);
	return strlen( buf );
}

static ssize_t xxx_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
	printk( "write %ld\n", (long)count );
	strncpy( buf_msg, buf, count );
	buf_msg[ count ] = '\0';
	return count;
}

CLASS_ATTR_RW( xxx );

static struct class *x_class;

int __init x_init(void) {
	int res;
	tv = kmalloc(sizeof(*tv),GFP_KERNEL);
	buf_msg = kmalloc(LEN_MSG+1,GFP_KERNEL | __GFP_ZERO);
	tv->tv_sec = 0;
	tv->tv_usec = 0;
	x_class = class_create( THIS_MODULE, "x-class" );
	if( IS_ERR( x_class ) ) printk( "bad class create\n" );
	res = class_create_file( x_class, &class_attr_xxx );
	printk( "'xxx' module initialized\n" );
	return res;
}

void x_cleanup(void) {
	class_remove_file( x_class, &class_attr_xxx );
	class_destroy( x_class );
	kfree(buf_msg);
	kfree(tv);
	return;
}

module_init( x_init );
module_exit( x_cleanup );

MODULE_LICENSE( "GPL" );
