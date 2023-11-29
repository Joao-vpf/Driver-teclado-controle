#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/random.h>
#include <asm/io.h>  

#define KBD_DATA_REG 0x60 
#define IRQ_HANDLED 1
#define IRQ_CONTROLE 2


static struct input_dev *input_dev;
static struct input_dev *input_control;

int random_range(int min, int max, int* x)
{
    unsigned int random_value;
    int new_random;

    do {
        get_random_bytes(&random_value, sizeof(random_value));
        new_random = min + (random_value % (max - min + 1));
    } while (new_random == *x);

    *x = new_random;
    return *x;
}

int altW=1,altS=1,altA=1,altD=1;

/* INICIO TECLADO */
static irqreturn_t keylogger_interrupt(int irq, void *dev_id)
{
    char scancode;

    scancode = inb(KBD_DATA_REG); 
	
	printk(KERN_INFO "%x\n", scancode);

    unsigned int valorHex = scancode;
    
    if (valorHex == 0xFFFFFFB9) 
    {
        // Tecla de espaço solta
        printk(KERN_INFO "Barra de Espaco solta\n"); //teste
        input_event(input_control, EV_KEY, BTN_SOUTH, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x39)
    {
        // Tecla de espaço pressionada
        printk(KERN_INFO "Barra de Espaco pressionada\n"); //teste
        input_event(input_control, EV_KEY, BTN_SOUTH, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }

	//AWSD
	
	//A
	if(scancode ==  0xFFFFFF9E)
    {
        // Tecla do A solta
        printk(KERN_INFO " Tecla do A solta\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY, 0);    
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x1E)
    {
        // Tecla do A pressionada
        printk(KERN_INFO " Tecla do A pressionada\n");// teste
        input_event(input_control, EV_ABS, ABS_RY, random_range(-10, 10, &altA));
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, -32767);
        input_sync(input_control);
	}
	
	//W
	if(scancode ==  0xFFFFFF91)
    {
        // Tecla do W solta
        printk(KERN_INFO " Tecla do W solta\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY, 0);  
        input_sync(input_control);  
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x11)
    {
        // Tecla do W pressionada
        printk(KERN_INFO " Tecla do W pressionada\n");// teste
        input_event(input_control, EV_ABS, ABS_RY, -32767);
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX,  random_range(-10, 10, &altW));
        input_sync(input_control);
	}
	
	//D
	if(scancode ==  0xFFFFFFA0)
    {
        // Tecla do D solta
        printk(KERN_INFO " Tecla do D solta\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY, 0);   
        input_sync(input_control); 
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x20)
    {
        // Tecla do W pressionada
        printk(KERN_INFO " Tecla do D pressionada\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY,random_range(-10, 10 ,&altD));
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, 32767);
        input_sync(input_control);
	}
	
	//S
	if(scancode ==  0xFFFFFF9F)
    {
        // Tecla do S solta
        printk(KERN_INFO " Tecla do S solta\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY, 0);   
        input_sync(input_control); 
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x1F)
    {
        // Tecla do S pressionada
        printk(KERN_INFO " Tecla do S pressionada\n"); //teste
        input_event(input_control, EV_ABS, ABS_RY, 32767);
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, random_range(-10, 10, &altS));
        input_sync(input_control);
	}
	
	
		
	
	 input_sync(input_control);
    return IRQ_HANDLED;
}

/* FIM TECLADO */


/* INICIO CONTROLE */

void setup_controller_events(struct input_dev *input_control)
{
  // Configura eventos específicos de botões para o dispositivo de controle
    set_bit(EV_KEY, input_control->evbit);
    set_bit(BTN_SOUTH, input_control->keybit);
    set_bit(BTN_EAST, input_control->keybit);
    set_bit(BTN_NORTH, input_control->keybit);
    set_bit(BTN_WEST, input_control->keybit);
    set_bit(BTN_TL, input_control->keybit);
    set_bit(BTN_TR, input_control->keybit);
    set_bit(BTN_SELECT, input_control->keybit);
    set_bit(BTN_START, input_control->keybit);
    set_bit(BTN_MODE, input_control->keybit);
    set_bit(BTN_THUMBL, input_control->keybit);
    set_bit(BTN_THUMBR, input_control->keybit);

    // Configura eventos específicos de eixos para o dispositivo de controle
    set_bit(EV_ABS, input_control->evbit);
    input_set_abs_params(input_control, ABS_X, -32768, 32767, 16, 128);
    
    input_set_abs_params(input_control, ABS_Y, -32768, 32767, 16, 128);
    input_set_abs_params(input_control, ABS_Z, 0, 1023, 0, 0);
    input_set_abs_params(input_control, ABS_RX, -32768, 32767, 16, 128);
    input_set_abs_params(input_control, ABS_RY, -32768, 32767, 16, 128);
    input_set_abs_params(input_control, ABS_RZ, 0, 1023, 0, 0);
    input_set_abs_params(input_control, ABS_HAT0X, -1, 1, 0, 0);
    input_set_abs_params(input_control, ABS_HAT0Y, -1, 1, 0, 0);

    // Configura eventos específicos de força de feedback para o dispositivo de controle
    set_bit(EV_FF, input_control->evbit);
    set_bit(FF_RUMBLE, input_control->ffbit);
    set_bit(FF_PERIODIC, input_control->ffbit);
    set_bit(FF_SQUARE, input_control->ffbit);
    set_bit(FF_TRIANGLE, input_control->ffbit);
    set_bit(FF_SINE, input_control->ffbit);
    set_bit(FF_GAIN, input_control->ffbit);

}

/* FIM CONTROLE */




static int __init keylogger_init(void)
{
    // Cria um dispositivo de entrada para teclado
    input_dev = input_allocate_device();
    if (!input_dev)
    {
        printk(KERN_ERR "keylogger: Falha ao alocar dispositivo de entrada\n");
        return -ENOMEM;
    }

    // Configura o dispositivo de entrada
    set_bit(EV_KEY, input_dev->evbit);

    // Registra a interrupção do teclado
    if (request_irq(1, keylogger_interrupt, IRQF_SHARED, "keylogger", input_dev))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar a interrupção do teclado\n");
        input_free_device(input_dev);
        return -EBUSY;
    }
    
    //Procura dispostivo
    input_dev->name = "AT Translated Set 2 keyboard";
    
    // Registra o dispositivo de entrada teclado
    if (input_register_device(input_dev))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar o dispositivo de entrada teclado\n");
        free_irq(1, input_dev);
        input_free_device(input_dev);
        return -EINVAL;
    }
    
    // Cria um dispositivo de entrada para controle
    input_control = input_allocate_device();
    if (!input_control)
    {
        printk(KERN_ERR "keylogger: Falha ao alocar dispositivo de entrada do controle\n");
        return -ENOMEM;
    }
    
    setup_controller_events(input_control);
  
    
    
    // Procura dispostivo
    input_control->name = "Microsoft X-Box One pad (Firmware 2015)";
    input_control->id.bustype = BUS_USB;
    input_control->id.vendor = 0x45e; 
    input_control->id.product = 0x2dd; 
    input_control->id.version = 0x203; 

    
    // Registra o dispositivo de entrada controle
    if (input_register_device(input_control))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar o dispositivo de entrada controle\n");
        free_irq(1, input_control);
        input_free_device(input_control);
        return -EINVAL;
    }
    
    printk(KERN_INFO "keylogger: Driver carregado\n");
    return 0;
}

static void __exit keylogger_exit(void)
{

   // Desregistrar o dispositivo de controle
    input_unregister_device(input_control);

    // Liberar a interrupção do controle
    free_irq(IRQ_CONTROLE, input_control);

    // Desregistrar o dispositivo de entrada do teclado
    input_unregister_device(input_dev);

    // Liberar a interrupção do teclado
    free_irq(1, input_dev);

    printk(KERN_INFO "keylogger: Driver descarregado\n");
}



module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Portin");
MODULE_DESCRIPTION("Driver de cria");
