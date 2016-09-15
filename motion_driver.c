#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

static unsigned int gpio=24;
static unsigned int irqNumber;

static irq_handler_t m_irq_handler(unsigned int irq,void *dev_id,struct pt_regs *regs);



static int  __init m_init(void)
{
	int result=0;
	printk(KERN_INFO "GPIO_TEST: initializing the GPIO_TEST LKM\n");
	if(!gpio_is_valid(gpio)){
	printk(KERN_INFO "GPIO_TEST: invalid GPIO\n");
	return -ENODEV;
	}

	gpio_request(gpio,"sysfs");
	gpio_direction_output(gpio,1);  //set gpio in input mode
	gpio_export(gpio,false);


	irqNumber=gpio_to_irq(gpio);
	printk(KERN_INFO "GPIO_TEST: gpio is mapped to IRQ:%d\n",irqNumber);

	//request an interrupt line//
	result = request_irq(irqNumber,(irq_handler_t)m_irq_handler,IRQF_TRIGGER_RISING,"motion_gpio_handler",NULL);

        printk(KERN_INFO "GPIO_TEST: the interrupt request result is:%d\n",result);
	return result;
}
static void __exit m_exit(void){
	printk(KERN_INFO "GPIO_TEST: the gpio state is currently:%d\n",gpio_get_value(gpio));
	gpio_set_value(gpio,0);
        gpio_unexport(gpio);
	free_irq(irqNumber,NULL);
	gpio_free(gpio);
	printk(KERN_INFO "GPIO_TEST:goodbby from the LKM\n");
	}

static irq_handler_t m_irq_handler(unsigned int irq,void *dev_id,struct pt_regs *regs)
{
	static unsigned int count;
	printk(KERN_INFO,"GPIO_TEST:Interrupt!",gpio_get_value(gpio));
	return (irq_handler_t) IRQ_HANDLED;
} 

module_init(m_init);
motion_exit(m_exit);

