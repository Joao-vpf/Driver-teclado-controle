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
#include <linux/semaphore.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <asm/io.h>  
#include <linux/miscdevice.h> 
#include <linux/fs.h>
#include <linux/uaccess.h>

#define KBD_DATA_REG 0x60 
#define IRQ_CONTROLE 1
#define IRQ_HANDLED 1

static struct semaphore sem;
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
int altE=1,altQ=1,altV=1,altB=1;

/* INICIO TECLADO */
static int my_keyboard_notify(struct notifier_block *nblock, unsigned long code, void *_param) 
{
	struct keyboard_notifier_param *param = _param;
	
	//Random range é chamado para imitar a imprecisão do usuario no controle e fazer o sistema continuar identificando o controle
    if (code == KBD_KEYCODE) 
    {
        down(&sem);
        if (param->down) 
        {		
        
            //Direcional esquerdo
            //E -> Direcional direita
            if(param->value ==  0x12)
			{
				printk(KERN_INFO "E -> Direcional direita");
				input_event(input_control, EV_ABS, ABS_Y, random_range(-10, 10, &altE));
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, -32767);
				input_sync(input_control);
			}
			
			//Q -> Direcional esquerda
			if(param->value  ==  0x10)
			{
				printk(KERN_INFO "Q -> Direcional esquerda");
				input_event(input_control, EV_ABS, ABS_Y,random_range(-10, 10 ,&altQ));
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, 32767);
				input_sync(input_control);
			}
			
			//V -> Direcional cima
            if(param->value ==  0x2F)
			{
				printk(KERN_INFO "V -> Direcional cima");
				input_event(input_control, EV_ABS, ABS_Y, -32767);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X,  random_range(-10, 10, &altV));
				input_sync(input_control);
			}
			
			//B -> Direcional baixo
			if(param->value  ==  0x30)
			{
				printk(KERN_INFO "B -> Direcional baixo");
				input_event(input_control, EV_ABS, ABS_Y, 32767);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, random_range(-10, 10, &altB));
				input_sync(input_control);
			}
			
			//Direcional direito
			
			//A -> Direcional direita
			if(param->value ==  0x1E)
			{
				printk(KERN_INFO "A -> Direcional direita");
				// Tecla do A pressionada
				input_event(input_control, EV_ABS, ABS_RY, random_range(-10, 10, &altA));
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_RX, -32767);
				input_sync(input_control);
			}
			
			//W -> Direcional cima
			if(param->value  ==  0x11)
			{
				printk(KERN_INFO "W -> Direcional cima");
				// Tecla do W pressionada
				input_event(input_control, EV_ABS, ABS_RY, -32767);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_RX,  random_range(-10, 10, &altW));
				input_sync(input_control);
			}
			
			//S -> Direcional baixo
			if(param->value == 0x1F)
			{
				printk(KERN_INFO "S -> Direcional baixo");
				// Tecla do S pressionada
				input_event(input_control, EV_ABS, ABS_RY, 32767);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_RX, random_range(-10, 10, &altS));
				input_sync(input_control);
			}
			
			//D -> Direcional esquerda
			if(param->value == 0x20)
			{
				printk(KERN_INFO "D -> Direcional esquerda");
				// Tecla do D pressionada
				input_event(input_control, EV_ABS, ABS_RY,random_range(-10, 10 ,&altD));
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_RX, 32767);
				input_sync(input_control);
			}
			
			//Botões superiores direita
			
			// ESPACO -> A
			if(param->value == 0x39)
			{
				printk(KERN_INFO "ESPACO -> A");
				// Tecla do ESPACO pressionada
				input_event(input_control, EV_KEY, BTN_SOUTH, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// Z -> B
			if(param->value == 0x2C)
			{
				printk(KERN_INFO "Z -> B");
				// Tecla do E pressionada
				input_event(input_control, EV_KEY, BTN_EAST, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// X -> X
			if(param->value == 0x2D)
			{
				printk(KERN_INFO "X -> X");
				// Tecla do Q pressionada
				input_event(input_control, EV_KEY, BTN_NORTH, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// F -> Y
			if(param->value == 0x21)
			{
				printk(KERN_INFO "F -> Y");
				// Tecla do F pressionada
				input_event(input_control, EV_KEY, BTN_WEST, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			//Setas do controle
			
			//Arrow up -> Seta cima
			if(param->value == 0x48)
			{
				printk(KERN_INFO "Arrow up -> Seta cima");
				// Tecla do Arrow up pressionada
				input_event(input_control, EV_ABS, ABS_HAT0Y, -1);
				input_sync(input_control);
			}
			
			//Arrow down -> Seta baixo
			if(param->value == 0x50)
			{
				printk(KERN_INFO "Arrow down -> Seta cima");
				// Tecla do Arrow down pressionada
				input_event(input_control, EV_ABS, ABS_HAT0Y, 1);
				input_sync(input_control);
			}
			
			//Arrow right -> Seta direita
			if(param->value == 0x4D)
			{
				printk(KERN_INFO "Arrow right -> Seta cima");
				// Tecla do Arrow right pressionada
				input_event(input_control, EV_ABS, ABS_HAT0X, 1);
				input_sync(input_control);
			}
			
			//Arrow left -> Seta esquerda
			if(param->value == 0x4B)
			{
				printk(KERN_INFO "Arrow left -> Seta cima");
				// Tecla do Arrow left pressionada
				input_event(input_control, EV_ABS, ABS_HAT0X, -1);
				input_sync(input_control);
			}
			
			// Botões superiores meio
			
			// NUM LOCK -> START
			if(param->value == 0x45)
			{
				printk(KERN_INFO "NUM LOCK -> Seta cima");
				// Tecla do NUM LOCK pressionada
				input_event(input_control, EV_KEY, BTN_START, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// END -> MENU
			if(param->value == 0x6B)
			{
				printk(KERN_INFO "END -> Seta cima");
				// Tecla do END pressionada
				input_event(input_control, EV_KEY, BTN_SELECT, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// HOME -> XBOX
			if(param->value == 0x66)
			{
				printk(KERN_INFO "HOME -> Seta cima");
				// Tecla do HOME pressionada
				input_event(input_control, EV_KEY, BTN_MODE, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// Gatilhos

			// 1 -> LB
			if(param->value == 0x2)
			{
				printk(KERN_INFO "1 -> LB");
				// Tecla do 1 pressionada
				input_event(input_control, EV_KEY, BTN_TL, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// 2 -> RB
			if(param->value == 0x3)
			{
				printk(KERN_INFO "2 -> RB");
				// Tecla do 2 pressionada
				input_event(input_control, EV_KEY, BTN_TR, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// 3 -> LT
			if(param->value == 0x4)
			{
				printk(KERN_INFO "3 -> LT");
				// Tecla do 3 pressionada
				input_event(input_control, EV_KEY, BTN_THUMBL, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// 4 -> RT
			if(param->value == 0x5)
			{
				printk(KERN_INFO "4 -> RT");
				// Tecla do 4 pressionada
				input_event(input_control, EV_KEY, BTN_THUMBR, 1);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			
			
			input_sync(input_control);
        }
        else
        {
            //Direcional esquerdo
            
            //E -> Direcional direita
            if(param->value ==  0x12)
			{
				input_event(input_control, EV_ABS, ABS_Y, 0);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, 0);
				input_sync(input_control);
			}
			
			//Q -> Direcional esquerda
			if(param->value  ==  0x10)
			{
				input_event(input_control, EV_ABS, ABS_Y, 0);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, 0);
				input_sync(input_control);
			}
			
			//V -> Direcional cima
            if(param->value ==  0x2F)
			{
				input_event(input_control, EV_ABS, ABS_Y, 0);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, 0);
				input_sync(input_control);
			}
			
			//B -> Direcional baixo
			if(param->value  ==  0x30)
			{
				input_event(input_control, EV_ABS, ABS_Y, 0);
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_X, 0);
				input_sync(input_control);
			}
			
            //Direcional direito 
			//A -> Direcional direita
			if(param->value ==  0x1E)
			{
				// Tecla do A solta
				input_event(input_control, EV_ABS, ABS_RY, 0);    
				input_sync(input_control);
				input_event(input_control, EV_ABS, ABS_RX, 0);
				input_sync(input_control);
			}
			
			//W -> Direcional cima
			if(param->value ==  0x11)
			{
				// Tecla do W solta
				input_event(input_control, EV_ABS, ABS_RY, 0);  
				input_sync(input_control);  
				input_event(input_control, EV_ABS, ABS_RX, 0);
				input_sync(input_control);
			}
			
			//S -> Direcional baixo
			if(param->value == 0x1F)
			{
				// Tecla do S solta
				input_event(input_control, EV_ABS, ABS_RY, 0);   
				input_sync(input_control); 
				input_event(input_control, EV_ABS, ABS_RX, 0);
				input_sync(input_control);
			}
			
			//D -> Direcional esquerda
			if(param->value == 0x20)
			{
				// Tecla do D solta
				input_event(input_control, EV_ABS, ABS_RY, 0);   
				input_sync(input_control); 
				input_event(input_control, EV_ABS, ABS_RX, 0);
				input_sync(input_control);
			}

			//Botões superiores direita
			
			// ESPACO -> A
			if (param->value == 0x39) 
			{
				// Tecla do ESPACO solta
				input_event(input_control, EV_KEY, BTN_SOUTH, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// Z -> B
			if (param->value == 0x2C) 
			{
				// Tecla do Z solta
				input_event(input_control, EV_KEY, BTN_EAST, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// X -> X
			if (param->value == 0x2D) 
			{
				// Tecla do X solta
				input_event(input_control, EV_KEY, BTN_NORTH, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// F -> Y
			if (param->value == 0x21) 
			{
				// Tecla do F solta
				input_event(input_control, EV_KEY, BTN_WEST, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			//Setas do controle
			
			//Arrow up -> Seta cima
			if(param->value == 0x48)
			{
				// Tecla do Arrow up solta
				input_event(input_control, EV_ABS, ABS_HAT0Y, 0);
				input_sync(input_control);
			}
			
			//Arrow down -> Seta baixo
			if(param->value == 0x50)
			{
				// Tecla do Arrow down solta
				input_event(input_control, EV_ABS, ABS_HAT0Y, 0);
				input_sync(input_control);
			}

			//Arrow right -> Seta direita
			if(param->value == 0x4D)
			{
				// Tecla do Arrow right solta
				input_event(input_control, EV_ABS, ABS_HAT0X, 0);
				input_sync(input_control);
			}
			
			//Arrow left -> Seta esquerda
			if(param->value == 0x4B)
			{
				// Tecla do Arrow left solta
				input_event(input_control, EV_ABS, ABS_HAT0X, 0);
				input_sync(input_control);
			}

			// Botões superiores meio
			
			// NUM LOCK -> START
			if (param->value == 0x45) 
			{
				// Tecla do NUM LOCK solta
				input_event(input_control, EV_KEY, BTN_START, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// END -> MENU
			if (param->value == 0x6B) 
			{
				// Tecla do END solta
				input_event(input_control, EV_KEY, BTN_SELECT, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// HOME -> XBOX
			if (param->value == 0x66) 
			{
				// Tecla do HOME solta
				input_event(input_control, EV_KEY, BTN_MODE, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// Gatilhos

			// 1 -> LB
			if (param->value == 0x2) 
			{
				// Tecla do 1 solta
				input_event(input_control, EV_KEY, BTN_TL, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// 2 -> RB
			if (param->value == 0x3) 
			{
				// Tecla do 2 solta
				input_event(input_control, EV_KEY, BTN_TR, 0);  // 0 indica que a tecla foi solta
				input_sync(input_control);
			} 
			
			// 3 -> LT
			if(param->value == 0x4)
			{
				input_event(input_control, EV_KEY, BTN_THUMBL, 0);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			
			// 4 -> RT
			if(param->value == 0x5)
			{
				input_event(input_control, EV_KEY, BTN_THUMBR, 0);  // 1 indica que a tecla foi pressionada
				input_sync(input_control);
			   
			}
			input_sync(input_control);
		}
        up(&sem);
    }

    return NOTIFY_OK;
}

static struct notifier_block my_keyboard_notifier = {
    .notifier_call = my_keyboard_notify
};

/* FIM TECLADO */


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
    set_bit(BTN_THUMBL, input_control->keybit); //LT
    set_bit(BTN_THUMBR, input_control->keybit); //RT

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
    //Configuração para teclado
    sema_init(&sem, 1);
    register_keyboard_notifier(&my_keyboard_notifier);

    
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

    // Desregistra o notificador de teclado
    unregister_keyboard_notifier(&my_keyboard_notifier);
    
    // Desregistrar o dispositivo de controle
    input_unregister_device(input_control);

    // Libera a interrupção do controle
    free_irq(IRQ_CONTROLE, input_control);


    printk(KERN_INFO "keylogger: Driver teclado/mouse-> controle descarregado\n");
}



module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Portin");
MODULE_DESCRIPTION("Driver de cria");
