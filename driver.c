/*
 * Autor: Portin
 * Licença: GPL-3.0
 *
 * Este programa é software livre; você pode redistribuí-lo e/ou modificá-lo
 * nos termos da Licença Pública Geral GNU, conforme publicada pela Free
 * Software Foundation; tanto a versão 3 da Licença, ou (a seu critério) qualquer
 * versão posterior.
 *
 * Este programa é distribuído na esperança de que possa ser útil, mas SEM
 * NENHUMA GARANTIA; sem uma garantia implícita de COMERCIALIZAÇÃO ou de
 * ADEQUAÇÃO A UM PROPÓSITO PARTICULAR. Consulte a Licença Pública Geral GNU
 * para obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
 * programa; se não, acesse http://www.gnu.org/licenses/gpl-3.0.html ou escreva para
 * a Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/random.h>
#include <linux/fcntl.h>
#include <linux/unistd.h>
#include <asm/io.h>  

#define KBD_DATA_REG 0x60 
#define IRQ_HANDLED 1
#define IRQ_CONTROLE 1
#define IRQ_MOUSE 1
#define IRQ_TECLADO 1


static struct input_dev *input_teclado;
static struct input_dev *input_mouse;
static struct input_dev *input_control;

int random_range(int min, int max, int* x)
{//Função que cria um valor aletario no range do min até o maximo
    unsigned int random_value;
    int new_random;

    do {
        get_random_bytes(&random_value, sizeof(random_value));
        new_random = min + (random_value % (max - min + 1));
    } while (new_random == *x);

    *x = new_random;
    return *x;
}

//Posição da tecla
int altW=1,altS=1,altA=1,altD=1;

/* INICIO TECLADO */
static irqreturn_t keylogger_interrupt(int irq, void *dev_id)
{
    char scancode;

    scancode = inb(KBD_DATA_REG); 

	//Direcional direito
	//Random range é chamado para imitar a imprecisão do usuario no controle e fazer o sistema continuar identificando o controle
	
	//A -> Direcional direita
	if(scancode ==  0xFFFFFF9E)
    {
        // Tecla do A solta
        input_event(input_control, EV_ABS, ABS_RY, 0);    
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x1E)
    {
		printk(KERN_INFO "A -> Direcional direita");
        // Tecla do A pressionada
        input_event(input_control, EV_ABS, ABS_RY, random_range(-10, 10, &altA));
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, -32767);
        input_sync(input_control);
	}
	
	//W -> Direcional cima
	if(scancode ==  0xFFFFFF91)
    {
        // Tecla do W solta
        input_event(input_control, EV_ABS, ABS_RY, 0);  
        input_sync(input_control);  
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x11)
    {
		printk(KERN_INFO "W -> Direcional direita");
        // Tecla do W pressionada
        input_event(input_control, EV_ABS, ABS_RY, -32767);
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX,  random_range(-10, 10, &altW));
        input_sync(input_control);
	}
	
	//S -> Direcional baixo
	if(scancode ==  0xFFFFFF9F)
    {
        // Tecla do S solta
        input_event(input_control, EV_ABS, ABS_RY, 0);   
        input_sync(input_control); 
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x1F)
    {
		printk(KERN_INFO "S -> Direcional direita");
        // Tecla do S pressionada
        input_event(input_control, EV_ABS, ABS_RY, 32767);
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, random_range(-10, 10, &altS));
        input_sync(input_control);
	}
	
	//D -> Direcional esquerda
	if(scancode ==  0xFFFFFFA0)
    {
        // Tecla do D solta
        input_event(input_control, EV_ABS, ABS_RY, 0);   
        input_sync(input_control); 
        input_event(input_control, EV_ABS, ABS_RX, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x20)
    {
		printk(KERN_INFO "D -> Direcional direita");
        // Tecla do D pressionada
        input_event(input_control, EV_ABS, ABS_RY,random_range(-10, 10 ,&altD));
        input_sync(input_control);
        input_event(input_control, EV_ABS, ABS_RX, 32767);
        input_sync(input_control);
	}
	
	//Botões superiores direita
	
	// ESPACO -> A
	if (scancode == 0xFFFFFFB9) 
    {
        // Tecla do ESPACO solta
        input_event(input_control, EV_KEY, BTN_SOUTH, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x39)
    {
		printk(KERN_INFO "ESPACO -> A");
        // Tecla do ESPACO pressionada
        input_event(input_control, EV_KEY, BTN_SOUTH, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// E -> B
	if (scancode == 0xFFFFFF92) 
    {
        // Tecla do E solta
        input_event(input_control, EV_KEY, BTN_EAST, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x12)
    {
		printk(KERN_INFO "E -> B");
        // Tecla do E pressionada
        input_event(input_control, EV_KEY, BTN_EAST, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// Q -> X
	if (scancode == 0xFFFFFF90) 
    {
        // Tecla do Q solta
        input_event(input_control, EV_KEY, BTN_NORTH, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x10)
    {
		printk(KERN_INFO "Q -> X");
        // Tecla do Q pressionada
        input_event(input_control, EV_KEY, BTN_NORTH, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// F -> Y
	if (scancode == 0xFFFFFFA1) 
    {
        // Tecla do F solta
        input_event(input_control, EV_KEY, BTN_WEST, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x21)
    {
		printk(KERN_INFO "F -> Y");
        // Tecla do F pressionada
        input_event(input_control, EV_KEY, BTN_WEST, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
    //Setas do controle
	
	//Arrow up -> Seta cima
	if(scancode ==  0xFFFFFFC8)
    {
        // Tecla do Arrow up solta
        input_event(input_control, EV_ABS, ABS_HAT0Y, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x48)
    {
		printk(KERN_INFO "Arrow up -> Seta cima");
        // Tecla do Arrow up pressionada
        input_event(input_control, EV_ABS, ABS_HAT0Y, -1);
        input_sync(input_control);
	}
	
	//Arrow down -> Seta baixo
	if(scancode ==  0xFFFFFFD0)
    {
        // Tecla do Arrow down solta
        input_event(input_control, EV_ABS, ABS_HAT0Y, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x50)
    {
		printk(KERN_INFO "Arrow down -> Seta cima");
        // Tecla do Arrow down pressionada
        input_event(input_control, EV_ABS, ABS_HAT0Y, 1);
        input_sync(input_control);
	}
	
	//Arrow right -> Seta direita
	if(scancode ==  0xFFFFFFCD)
    {
        // Tecla do Arrow right solta
        input_event(input_control, EV_ABS, ABS_HAT0X, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x4D)
    {
		printk(KERN_INFO "Arrow right -> Seta cima");
        // Tecla do Arrow right pressionada
        input_event(input_control, EV_ABS, ABS_HAT0X, 1);
        input_sync(input_control);
	}
	
	//Arrow left -> Seta esquerda
	if(scancode ==  0xFFFFFFCB)
    {
        // Tecla do Arrow left solta
        input_event(input_control, EV_ABS, ABS_HAT0X, 0);
        input_sync(input_control);
	}

    if(scancode ==  0x4B)
    {
		printk(KERN_INFO "Arrow left -> Seta cima");
        // Tecla do Arrow left pressionada
        input_event(input_control, EV_ABS, ABS_HAT0X, -1);
        input_sync(input_control);
	}
	
	// Botões superiores meio
	
	// NUM LOCK -> START
	if (scancode == 0XFFFFFFC5) 
    {
        // Tecla do NUM LOCK solta
        input_event(input_control, EV_KEY, BTN_START, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x45)
    {
		printk(KERN_INFO "NUM LOCK -> Seta cima");
        // Tecla do NUM LOCK pressionada
        input_event(input_control, EV_KEY, BTN_START, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// END -> MENU
	if (scancode == 0XFFFFFFCF) 
    {
        // Tecla do END solta
        input_event(input_control, EV_KEY, BTN_SELECT, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x4f)
    {
		printk(KERN_INFO "END -> Seta cima");
        // Tecla do END pressionada
        input_event(input_control, EV_KEY, BTN_SELECT, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// HOME -> XBOX
	if (scancode == 0xFFFFFFC7) 
    {
        // Tecla do HOME solta
        input_event(input_control, EV_KEY, BTN_MODE, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x47)
    {
		printk(KERN_INFO "HOME -> Seta cima");
        // Tecla do HOME pressionada
        input_event(input_control, EV_KEY, BTN_MODE, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
	
	// Gatilhos

	// 1 -> LB
	if (scancode == 0xFFFFFF82) 
    {
        // Tecla do 1 solta
        input_event(input_control, EV_KEY, BTN_TL, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x2)
    {
		printk(KERN_INFO "1 -> LB");
        // Tecla do 1 pressionada
        input_event(input_control, EV_KEY, BTN_TL, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	// 2 -> RB
	if (scancode == 0xFFFFFF83) 
    {
        // Tecla do 2 solta
        input_event(input_control, EV_KEY, BTN_TR, 0);  // 0 indica que a tecla foi solta
        input_sync(input_control);
    } 
    if(scancode == 0x3)
    {
		printk(KERN_INFO "2 -> RB");
        // Tecla do 2 pressionada
        input_event(input_control, EV_KEY, BTN_TR, 1);  // 1 indica que a tecla foi pressionada
        input_sync(input_control);
       
    }
    
	
	input_sync(input_control);
    return IRQ_HANDLED;
}

/* FIM TECLADO */

/* INICIO MOUSE */

unsigned char read_from_keyboard_port(void)
{
  int fd;
    struct input_event ie;

    if((fd = open("/dev/input/mice", O_RDONLY)) == -1) {
        perror("opening device");
        exit(EXIT_FAILURE);
    }

    while(read(fd, &ie, sizeof(struct input_event))) {
        print("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
               ie.time.tv_sec, ie.time.tv_usec, ie.type,
               ie.code, ie.value);
    }
    return IRQ_HANDLED;
}
/* FIM MOUSE */


/* INICIO CONTROLE */

void setup_controller_events(struct input_dev *input_control)
{
  // Configura eventos específicos de botões para o dispositivo de controle
    set_bit(EV_KEY, input_control->evbit);
    set_bit(BTN_SOUTH, input_control->keybit); //A
    set_bit(BTN_EAST, input_control->keybit); //B
    set_bit(BTN_NORTH, input_control->keybit); //X
    set_bit(BTN_WEST, input_control->keybit); //Y
    set_bit(BTN_TL, input_control->keybit); //LB
    set_bit(BTN_TR, input_control->keybit); //RB
    set_bit(BTN_SELECT, input_control->keybit); //MENU
    set_bit(BTN_START, input_control->keybit); //START
    set_bit(BTN_MODE, input_control->keybit); //XBOX
    set_bit(BTN_THUMBL, input_control->keybit);
    set_bit(BTN_THUMBR, input_control->keybit);

    // Configura eventos específicos de eixos para o dispositivo de controle
    set_bit(EV_ABS, input_control->evbit);
    input_set_abs_params(input_control, ABS_X, -32768, 32767, 16, 128);//Direcional esquerdo
    input_set_abs_params(input_control, ABS_Y, -32768, 32767, 16, 128);//Direcional esquerdo
    input_set_abs_params(input_control, ABS_Z, 0, 1023, 0, 0);//Direcional esquerdo
    input_set_abs_params(input_control, ABS_RX, -32768, 32767, 16, 128); //Direcional direito
    input_set_abs_params(input_control, ABS_RY, -32768, 32767, 16, 128); //Direcional direito
    input_set_abs_params(input_control, ABS_RZ, 0, 1023, 0, 0); //Direcional direito
    input_set_abs_params(input_control, ABS_HAT0X, -1, 1, 0, 0);//Setas
    input_set_abs_params(input_control, ABS_HAT0Y, -1, 1, 0, 0);//Setas

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
    input_teclado = input_allocate_device();
    if (!input_teclado)
    {
        printk(KERN_ERR "keylogger: Falha ao alocar dispositivo de entrada *Teclado*\n");
        return -ENOMEM;
    }

    // Configura o dispositivo de entrada
    set_bit(EV_KEY, input_teclado->evbit);

    // Registra a interrupção do teclado
    if (request_irq(IRQ_TECLADO, keylogger_interrupt, IRQF_SHARED, "keylogger", input_teclado))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar a interrupção do teclado\n");
        input_free_device(input_teclado);
        return -EBUSY;
    }
    
    //Registra um nome para o dispostivo
    input_teclado->name = "AT Translated Set 2 keyboard";
    
    // Registra o dispositivo de entrada teclado
    if (input_register_device(input_teclado))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar o dispositivo de entrada teclado\n");
        free_irq(IRQ_TECLADO, input_teclado);
        input_free_device(input_teclado);
        return -EINVAL;
    }
    
     // Cria um dispositivo de entrada para mouse
    input_mouse = input_allocate_device();
    if (!input_mouse)
    {
        printk(KERN_ERR "keylogger: Falha ao alocar dispositivo de entrada *Mouse*\n");
        return -ENOMEM;
    }

    // Configura o dispositivo de entrada
    set_bit(EV_REL, input_mouse->evbit);
	set_bit(REL_X, input_mouse->relbit);
	set_bit(REL_Y, input_mouse->relbit);
    set_bit(EV_KEY, input_mouse->evbit);
	set_bit(BTN_RIGHT, input_mouse->keybit);
	set_bit(BTN_LEFT, input_mouse->keybit);
	
		
    // Registra a interrupção do mouse
    if (request_irq(IRQ_MOUSE, keylogger_interrupt_mouse, IRQF_SHARED, "keylogger", input_mouse))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar a interrupção do mouse\n");
        input_free_device(input_mouse);
        return -EBUSY;
    }
    
    //Registra um nome para o dispostivo
    input_mouse->name = "ImExPS/2 Generic Explorer Mouse";
    
    // Registra o dispositivo de entrada mouse
    if (input_register_device(input_mouse))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar o dispositivo de entrada mouse\n");
        free_irq(IRQ_MOUSE, input_mouse);
        input_free_device(input_mouse);
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
    
    //Registra um nome para o dispostivo
    input_control->name = "Microsoft X-Box One pad (Firmware 2015)";
    input_control->id.bustype = BUS_USB;
    input_control->id.vendor = 0x45e; 
    input_control->id.product = 0x2dd; 
    input_control->id.version = 0x203; 

    
    // Registra o dispositivo de entrada controle
    if (input_register_device(input_control))
    {
        printk(KERN_ERR "keylogger: Falha ao registrar o dispositivo de entrada controle\n");
        free_irq(IRQ_CONTROLE, input_control);
        input_free_device(input_control);
        return -EINVAL;
    }
    
    printk(KERN_INFO "keylogger: Driver teclado/mouse-> controle carregado\n");
    return 0;
}

static void __exit keylogger_exit(void)
{

   // Desregistrar o dispositivo de controle
    input_unregister_device(input_control);

    // Liberar a interrupção do controle
    free_irq(IRQ_CONTROLE, input_control);
	
	   // Desregistrar o dispositivo de mouse
    input_unregister_device(input_mouse);

    // Liberar a interrupção do mouse
    free_irq(IRQ_MOUSE, input_mouse);

    // Desregistrar o dispositivo de entrada do teclado
    input_unregister_device(input_teclado);

    // Liberar a interrupção do teclado
    free_irq(IRQ_TECLADO, input_teclado);

    printk(KERN_INFO "keylogger: Driver teclado/mouse-> controle descarregado\n");
}



module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Portin");
MODULE_DESCRIPTION("Driver de cria");
