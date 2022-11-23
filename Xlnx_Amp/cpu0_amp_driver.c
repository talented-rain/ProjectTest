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
#include <linux/irqchip/arm-gic.h>
#include <asm/smp.h>

// ------------------------------------------------------------------------------
// The defines
#define CPU0_AMP_NAME                                       "cpu0_amp"

// ------------------------------------------------------------------------------
// The globals
volatile unsigned int __iomem *cpu_share_addr;

unsigned int SoftIntrIdToCpu0 = 0;
unsigned int SoftIntrIdToCpu1 = 1;

// ------------------------------------------------------------------------------
// The functions

// ------------------------------------------------------------------------------
// API functions
//!\brief       cpu0_amp_driver_soft_isr
//!\param[in]   None
//!\param[out]  None
//!\note        Cpu0 Software Interrupt
static void cpu0_amp_driver_soft_isr(void)
{
    unsigned int Cpu1_Data;

    Cpu1_Data = *(cpu_share_addr);
    printk("%s: Get Information From CPU1: %d\n", __FUNCTION__, Cpu1_Data);

    *(cpu_share_addr) = Cpu1_Data + 99;
    gic_raise_softirq(cpumask_of(1), SoftIntrIdToCpu1);
}

//!\brief       cpu0_amp_driver_probe
//!\param[in]   None
//!\param[out]  None
//!\note        Cpu0 Driver Probe
static int cpu0_amp_driver_probe(struct platform_device *p_dev)
{
    struct resource *p_stu_resource;
    unsigned int resource_size;

    p_stu_resource = platform_get_resource(p_dev, IORESOURCE_MEM, 0);
    resource_size  = (p_stu_resource->end - p_stu_resource->start) + 1;
    cpu_share_addr = ioremap(p_stu_resource->start, resource_size);

    set_ipi_handler(SoftIntrIdToCpu0, cpu0_amp_driver_soft_isr, CPU0_AMP_NAME);

    return 0;
}

//!\brief       cpu0_amp_driver_remove
//!\param[in]   None
//!\param[out]  None
//!\note        Cpu0 Driver Remove
static int cpu0_amp_driver_remove(struct platform_device *p_dev)
{
    clear_ipi_handler(SoftIntrIdToCpu0);

    iounmap(cpu_share_addr);

    return 0;
}

// of_device_id
const struct of_device_id g_stu_cpu0_amp_table[] =
{
    {.compatible = "xlnx, z7-lite, cpu0_amp", },
};

// platform_driver
static struct platform_driver g_stu_cpu0_amp_platdriver =
{
    .probe = cpu0_amp_driver_probe,
    .remove = cpu0_amp_driver_remove,

    .driver = 
    {
        .name           = CPU0_AMP_NAME,
        .of_match_table = g_stu_cpu0_amp_table,
    },
};

//!\brief       cpu0_amp_driver_init
//!\param[in]   None
//!\param[out]  None
//!\note        driver init
static int __init cpu0_amp_driver_init(void)
{
    platform_driver_register(&g_stu_cpu0_amp_platdriver);

    return 0;
}

//!\brief       cpu0_amp_driver_exit
//!\param[in]   None
//!\param[out]  None
//!\note        driver exit
static void __exit cpu0_amp_driver_exit(void)
{
    platform_driver_unregister(&g_stu_cpu0_amp_platdriver);
}

// ------------------------------------------------------------------------------
module_init(cpu0_amp_driver_init);
module_exit(cpu0_amp_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yang Yujun <yujiantianhu@163.com>");

// end of file
