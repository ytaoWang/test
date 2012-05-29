#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x80567cab, "module_layout" },
	{ 0x32115e5, "kmem_cache_destroy" },
	{ 0x706aae9d, "cdev_del" },
	{ 0xaa5b0f7, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x62b53a39, "cdev_init" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x670c0597, "down_interruptible" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x2bc95bd4, "memset" },
	{ 0xfcf580f6, "current_task" },
	{ 0x50eedeb8, "printk" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x69e638d, "fasync_helper" },
	{ 0xb4390f9a, "mcount" },
	{ 0xb5e8694f, "kmem_cache_free" },
	{ 0xcf83da5, "cdev_add" },
	{ 0x10ed2d79, "kmem_cache_alloc" },
	{ 0x4292364c, "schedule" },
	{ 0x6e712077, "kmem_cache_alloc_trace" },
	{ 0x6479c34c, "kmem_cache_create" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0x57b09822, "up" },
	{ 0x4b8b3ae4, "kill_fasync" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B3CEDDDA0360A52466C0FB5");
