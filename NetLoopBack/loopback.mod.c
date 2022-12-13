#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x3e42eeaf, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9967f581, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x53d10f2e, __VMLINUX_SYMBOL_STR(ether_setup) },
	{ 0xe1769aea, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x688b8e0d, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0xbbe077cb, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xff49280b, __VMLINUX_SYMBOL_STR(__dev_kfree_skb_any) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xce09d270, __VMLINUX_SYMBOL_STR(netif_rx) },
	{ 0x176fd972, __VMLINUX_SYMBOL_STR(eth_type_trans) },
	{ 0x51d46bd0, __VMLINUX_SYMBOL_STR(skb_put) },
	{ 0xa25d3fab, __VMLINUX_SYMBOL_STR(__netdev_alloc_skb) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x272af399, __VMLINUX_SYMBOL_STR(netif_tx_wake_queue) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A8846D4D7CCE68304C43B91");
