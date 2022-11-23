/*
 * Author: Yang Yujun
 * E-mail: <yujiantianhu@163.com>
 * Created On:
 *         2022.11.19
 */

// ------------------------------------------------------------------------------
//!\file  cpu0_amp_driver.c
//!
//!\brief double core communication
// ------------------------------------------------------------------------------
// The includes
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>

// ------------------------------------------------------------------------------
// The defines
#define CPU0_AMP_NAME                                       "cpu0_amp"
#define SHARE_ADDR_BASE                                     0xFFFF0000

// ------------------------------------------------------------------------------
// The globals

// ------------------------------------------------------------------------------
// The functions

// ------------------------------------------------------------------------------
// API functions
//!\brief       cpu0_amp_device_release
//!\param[in]   None
//!\param[out]  None
//!\note        Cpu0 Device Release
static void cpu0_amp_device_release(struct device *dev)
{

}

// resource
static struct resource g_stu_resource[] =
{
    {.start = SHARE_ADDR_BASE, .end = SHARE_ADDR_BASE + 4 -1, .flags = IORESOURCE_MEM,},
};

// platform_device
static struct platform_device g_stu_cpu0_amp_platdevice = 
{
    .name          = CPU0_AMP_NAME,
    .resource      = g_stu_resource,
    .num_resources = ARRAY_SIZE(g_stu_resource),

    .dev  =
    {
        .release = cpu0_amp_device_release,
    },
};

//!\brief       cpu0_amp_device_init
//!\param[in]   None
//!\param[out]  None
//!\note        device init
static int __init cpu0_amp_device_init(void)
{
    platform_device_register(&g_stu_cpu0_amp_platdevice);

    return 0;
}

//!\brief       cpu0_amp_device_exit
//!\param[in]   None
//!\param[out]  None
//!\note        device exit
static void __exit cpu0_amp_device_exit(void)
{
    platform_device_unregister(&g_stu_cpu0_amp_platdevice);
}

// ------------------------------------------------------------------------------
module_init(cpu0_amp_device_init);
module_exit(cpu0_amp_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yang Yujun <yujiantianhu@163.com>");

// end of file
