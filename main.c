#include <linux/kernel.h>
#include <linux/kprobes.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

static char symbol[KSYM_NAME_LEN] = "kernel_clone";
module_param_string(symbol, symbol, KSYM_NAME_LEN, 0644);

static struct kprobe kp = {
    .symbol_name = symbol,
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    long arg1 = (long)regs->regs[0];

    pr_info("<%s>: pid [%d], arg1 [%lx] \n", p->symbol_name, current->pid, arg1);

    return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    /* EMPTY */
}

static int __init kprobe_init(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;

    ret = register_kprobe(&kp);

    if (ret < 0) {
        return ret;
    }

    return 0;
}

static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
}


module_init(kprobe_init);
module_exit(kprobe_exit);

MODULE_LICENSE("GPL");
