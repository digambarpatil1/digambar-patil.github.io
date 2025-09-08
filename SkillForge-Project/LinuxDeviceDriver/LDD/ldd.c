#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Digambar  ");
MODULE_DESCRIPTION("Our first dynamically loadable kernel module");
struct proc_dir_entry *entry;
char proc_buffer[] ="First driver read\n";

static ssize_t my_proc_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
    printk("Hello, World! (Write called from Digambar!!)\n");

    int bytes_to_copy = min(sizeof(proc_buffer) - 1, count);

    if (copy_from_user(proc_buffer, ubuf, bytes_to_copy))
        return -EFAULT;

    proc_buffer[bytes_to_copy] = '\0';  // Null-terminate
    return bytes_to_copy;
 }


// Read function
static ssize_t my_proc_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
   
   size_t proc_buffer_size =strlen(proc_buffer);

    if (*ppos > 0 || count < proc_buffer_size)
        return 0;
    
    printk("Hello, World! (Read called from Digambar!!)\n");
  
    if (copy_to_user(ubuf, proc_buffer, proc_buffer_size))
        return -EFAULT;
    *ppos = proc_buffer_size;
    return proc_buffer_size;
}

struct proc_ops my_proc_ops = {
    .proc_read = my_proc_read,
    .proc_write = my_proc_write,
};

static int pyjama_module_init(void) {
    printk("Hello, World! (from Digambar!!)\n");
    entry = proc_create("myprocfile", 0, NULL, &my_proc_ops);
    if (!entry) {
       pr_err("Failed to create /proc/myprocfile\n");
       return -1;
    }
    printk("Hello, World! (from Digambar exit!!)\n");
    return 0;
}

static void pyjama_module_exit(void) {
    proc_remove(entry);  // Removes the /proc entry
    printk("Good, Bye! (from Digambar!!)\n");
}

module_init(pyjama_module_init);
module_exit(pyjama_module_exit);