/*
 * Author: Yang Yujun
 * E-mail: <yujiantianhu@163.com>
 * Created on:
 *         2022.12.12
 */

//!-------------------------------------------------------------------------------------
//!\file    loopback.c
//!
//!\brief   Ether LoopBack Test
//!-------------------------------------------------------------------------------------
// The includes
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <asm/io.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>

// -------------------------------------------------------------------------------------
// The defines

// -------------------------------------------------------------------------------------
// The globals
struct net_device *p_stu_netdev = NULL;

// -------------------------------------------------------------------------------------
// The functions

// -------------------------------------------------------------------------------------
// Private Data
struct net_loopback_priv_t
{
    unsigned int tx_lenth;
    unsigned int rx_lenth;

    struct net_device_stats *net_stats;
};

static struct net_loopback_priv_t g_stu_netlooback_dat = 
{
    .tx_lenth = 32,
    .rx_lenth = 64,
};

// -------------------------------------------------------------------------------------
// API functions
// void ether_setup(struct net_device *dev)
// {
//     dev->header_ops		    = &eth_header_ops;
//     dev->type		        = ARPHRD_ETHER;
//     dev->hard_header_len    = ETH_HLEN;
//     dev->mtu		        = ETH_DATA_LEN;
//     dev->addr_len		    = ETH_ALEN;
//     dev->tx_queue_len	    = 1000;	/* Ethernet wants good queues */
//     dev->flags		        = IFF_BROADCAST|IFF_MULTICAST;
//     dev->priv_flags        |= IFF_TX_SKB_SHARING;

//     eth_broadcast_addr(dev->broadcast);
// }

static int net_looback_driver_open(struct net_device *dev)
{
    /* #define random_ether_addr(addr) eth_random_addr(addr) */
    random_ether_addr(dev->dev_addr);

    /* static inline void netif_start_queue(struct net_device *dev) */
    /* start tx */
    netif_start_queue(dev);

    dev->stats.rx_packets = 0;
    dev->stats.tx_packets = 0;
    dev->stats.rx_bytes = 0;
    dev->stats.tx_bytes = 0;

    printk("open net_lo device\n");

    return 0;
}

static int net_looback_driver_stop(struct net_device *dev)
{
    /* static inline void netif_stop_queue(struct net_device *dev) */
    netif_stop_queue(dev);

    dev->stats.rx_packets = 0;
    dev->stats.tx_packets = 0;
    dev->stats.rx_bytes = 0;
    dev->stats.tx_bytes = 0;

    printk("stop net_lo device\n");

    return 0;
}

static int net_looback_driver_start_recv(void *data, int size, struct net_device *dev)
{
//    struct net_loopback_priv_t *p_stu_priv;
    struct sk_buff *skb;
    void *skb_data;
    unsigned int lenth;

    struct ethhdr *p_ethhdr;
    struct iphdr  *p_iphdr;
    unsigned char dest_addr[ETH_ALEN] = {0};
    unsigned char src_addr[ETH_ALEN] = {0};
    unsigned int saddr, daddr;
    unsigned char *ptr;

    if (!data)
        return -1;

    lenth = size;
    if (lenth < (sizeof(struct ethhdr) + sizeof(struct iphdr)))
    {
        printk("lenth: %d, Data received is not valid, it should contain ip and tcp info!\n", lenth);
        return -1;
    }

    /* static inline void *netdev_priv(const struct net_device *dev) */
    // p_stu_priv = netdev_priv(dev);

    /* sk_buff
     * dev_alloc_skb申请一个全新的sk_buff结构体, 数据段全长为lenth + 4, 其中end指向尾部, head指向头部
     * sk_buff数据段包括: head, data, tail, end; data和tail之间即为完整的数据. 刚分配的sk_buff结构体, head = data = tail
     * skb_reserve(2), head固定不动, data、tail同时后挪2个字节, 此时data = head + 2
     * skb_put, data固定不动, tail后挪lenth个字节, 且skb->len = lenth, data和tail之间腾出一段长度为lenth的空间
     * 最终: head + 2 = data, data + lenth = tail, tail + 2 = end, 且end - head = lenth + 4 = sk_buff数据段的大小
     */
    /* static inline struct sk_buff *dev_alloc_skb(unsigned int length) */
    skb = dev_alloc_skb(lenth + 4);
    /* static inline void skb_reserve(struct sk_buff *skb, int len) */
    skb_reserve(skb, 2);
    /* unsigned char *skb_put(struct sk_buff *skb, unsigned int len); */
    skb_data = (void *)skb_put(skb, lenth);

    skb->dev       = dev;
    /* __be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev) */
    skb->protocol  = eth_type_trans(skb, dev);
    skb->ip_summed = CHECKSUM_UNNECESSARY;
    memcpy(skb_data, data, lenth);

    /* 回环模式, 模拟实际硬件, 将TCP/IP协议头剥离后再调换放回 */
    p_ethhdr = (struct ethhdr *)skb_data;
    p_iphdr  = (struct iphdr  *)(skb_data + sizeof(struct ethhdr));

    memcpy(dest_addr, p_ethhdr->h_source, sizeof(dest_addr));
    memcpy(src_addr, p_ethhdr->h_dest, sizeof(src_addr));
    memcpy(p_ethhdr->h_source, src_addr, sizeof(src_addr));
    memcpy(p_ethhdr->h_dest, dest_addr, sizeof(dest_addr));

    saddr = p_iphdr->daddr;
    daddr = p_iphdr->saddr;
    p_iphdr->daddr = daddr;
    p_iphdr->saddr = saddr;
    p_iphdr->check = 0;
    /* 校验 */
    p_iphdr->check = ip_fast_csum((unsigned char *)p_iphdr, p_iphdr->ihl);

    ptr = (unsigned char *)(skb_data + sizeof(struct ethhdr) + sizeof(struct iphdr));
    /* 这个字节, 0x00表示rely, 0x08表示ping */
    *ptr = 0;

    /* int netif_rx(struct sk_buff *skb); */
    netif_rx(skb);

    dev->last_rx = jiffies;
    dev->stats.rx_packets++;
    dev->stats.rx_bytes += lenth;

    return 0;
}

static netdev_tx_t net_looback_driver_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    unsigned int data_len;
    unsigned char *data;

    if (!skb)
        return 0;

    data_len = (skb->len > ETH_ZLEN) ? skb->len : ETH_ZLEN;
    data = (unsigned char *)kmalloc(data_len, GFP_KERNEL);
    if (!data)
        return 0;

    netif_stop_queue(dev);

    /* 从应用层下来的skb->data包含TCP/IP协议头 */
    memset(data, 0, data_len);
    memcpy(data, skb->data, skb->len);

    dev->trans_start = jiffies;
    dev->stats.tx_packets++;
    dev->stats.tx_bytes += data_len;

    /* 回环, 发送即接收 */
    net_looback_driver_start_recv(data, data_len, dev);
    printk("data_len = %d, tx data:\n %x\n", data_len, data);

    kfree(data);
    /* static inline void dev_kfree_skb_any(struct sk_buff *skb); */
    dev_kfree_skb_any(skb);

    netif_wake_queue(dev);

    return NETDEV_TX_OK;
}

static void net_looback_driver_tx_timeout(struct net_device *dev)
{
    /* static inline void netif_wake_queue(struct net_device *dev) */
    netif_wake_queue(dev);
}	

static int net_looback_driver_set_mac_address(struct net_device *dev, void *addr)
{
    return 0;
}

static struct net_device_ops g_stu_netops =
{
    .ndo_open = net_looback_driver_open,
    .ndo_stop = net_looback_driver_stop,

    .ndo_start_xmit = net_looback_driver_start_xmit,
    .ndo_tx_timeout = net_looback_driver_tx_timeout,

//    .ndo_set_mac_address = net_looback_driver_set_mac_address,
};

// init & exit
/* ----------------------------------------------------------------------------------- */
static int __init net_loopback_driver_init(void)
{
    int retval;

    /* #define alloc_netdev(sizeof_priv, name, name_assign_type, setup) */
    p_stu_netdev = alloc_netdev(sizeof(g_stu_netlooback_dat), "net_lo", NET_NAME_ENUM, ether_setup);
    if (!p_stu_netdev)
        goto fail1;

    printk("alloc_netdev success!\n");

    p_stu_netdev->flags     |= IFF_NOARP;
    p_stu_netdev->ml_priv    = &g_stu_netlooback_dat;
    p_stu_netdev->netdev_ops = &g_stu_netops;

    /* int register_netdev(struct net_device *dev); */
    retval = register_netdev(p_stu_netdev);
    if (0 > retval)
        goto fail2;

    printk("register_netdev success!\n");

    return 0;

fail2:
    free_netdev(p_stu_netdev);

fail1:
    return 0;
}

static void __exit net_loopback_driver_exit(void)
{
    /* void unregister_netdev(struct net_device *dev); */
    unregister_netdev(p_stu_netdev);

    /* void free_netdev(struct net_device *dev); */
    free_netdev(p_stu_netdev);
}

/* ----------------------------------------------------------------------------------- */
module_init(net_loopback_driver_init);
module_exit(net_loopback_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yang Yujun <yujiantianhu@163.com>");

// end of file
