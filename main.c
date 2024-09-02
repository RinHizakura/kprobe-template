#include <linux/kernel.h>
#include <linux/kprobes.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt, __func__

static char symbol[KSYM_NAME_LEN] = "kernel_clone";
module_param_string(symbol, symbol, KSYM_NAME_LEN, 0644);

static int kprobe_status = -1;
static int kretprobe_status = -1;

static struct kprobe kp = {
    .symbol_name = symbol,
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    long arg0 = (long) regs->regs[0];
    long arg1 = (long) regs->regs[1];
    long arg2 = (long) regs->regs[2];

    pr_info("<%s>: pid [%d]\n", p->symbol_name, current->pid);

    return 0;
}

static void handler_post(struct kprobe *p,
                         struct pt_regs *regs,
                         unsigned long flags)
{
    /* EMPTY */
}

static struct kretprobe kretp = {
    .kp.symbol_name = symbol,
    .data_size = 0,
    .maxactive = 20,
};

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    long retval = regs_return_value(regs);

    pr_info("%s: return=%lu\n", symbol, retval);
    return 0;
}
NOKPROBE_SYMBOL(ret_handler);

static int __init kprobe_init(void)
{
    int ret;

    kprobe_status = -1;
    kretprobe_status = -1;

    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    ret = register_kprobe(&kp);
    if (ret == 0)
        kprobe_status = 0;

    kretp.handler = ret_handler;
    kretp.entry_handler = NULL;
    ret = register_kretprobe(&kretp);
    if (ret == 0)
        kretprobe_status = 0;

    pr_info("kprobe=%d kretprobe=%d\n", kprobe_status, kretprobe_status);

    return kprobe_status + kretprobe_status;
}

static void __exit kprobe_exit(void)
{
    if (kprobe_status == 0)
        unregister_kprobe(&kp);

    if (kretprobe_status == 0)
        unregister_kretprobe(&kretp);
}


module_init(kprobe_init);
module_exit(kprobe_exit);

MODULE_LICENSE("GPL");
