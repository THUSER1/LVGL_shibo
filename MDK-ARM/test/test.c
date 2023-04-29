#include "test.h"
#include "main.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"
#include "math.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "stdio.h"
#include "ILI93xx.h"
#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"  
#include "touch.h" 
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "C:\Users\16004\Desktop\stm32\lvgl-8.2.0\examples\get_started\lv_example_get_started.h"
#include "C:\Users\16004\Desktop\stm32\lvgl-8.2.0\src\hal\lv_hal_indev.h"
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;
extern uint16_t sins[200];
	extern uint16_t fangbos[200];
	extern uint16_t	sanjiaobosu[200];
	int vcc=0;
	char vccs[20]={0};
	char vmaxs[20]={0};
	char vmins[20]={0};
lv_obj_t * sw;
lv_obj_t * scr;
lv_obj_t* roller;
lv_obj_t* slider;
lv_obj_t* slider_time;
lv_obj_t* label4;
static lv_obj_t *win1_contanier = NULL;
static lv_obj_t *win2_contanier = NULL;
static lv_point_t lines_th[320]={ 0 }; 
static lv_point_t frql1s_th[3]={ {110,-5},{110,190} }; 
static lv_point_t frql2s_th[3]={ {220,-5},{220,190} }; 
static lv_point_t trgls_th[3]={ {40,100},{300,100} }; 
static lv_point_t cenls_th[3]={ {180,0},{180,170} }; 
void show_win2();
 void shibo();
int flag;		
int dflag1=0,dflag2=0,dflag3=0;
int runflag=0,stopflag=0,frq1flag=0,frq2flag=0;
	int ms=0;
	uint32_t adcs[330]={0};
	int xemp[330]={0};
	int tempth[330]={0};	
lv_obj_t* label_vpp;
	lv_obj_t* label_vmax;
	lv_obj_t* label_vmin;
	lv_obj_t* label_timesld;
	lv_obj_t* line;
	lv_obj_t* frql1;
	lv_obj_t* frql2;
	lv_obj_t* trgl;
	lv_obj_t* btn_time;
	lv_obj_t* btn_time2;
	lv_obj_t* btn_run;
	lv_obj_t* btn_stop;
	lv_obj_t* btn_left1;
	lv_obj_t* btn_right1;
	lv_obj_t* btn_left2;
	lv_obj_t* btn_right2;
	lv_obj_t* label_frq;
	lv_obj_t* btn_trigger;
	lv_obj_t* btn_up;
	lv_obj_t* btn_down;
void my_event(lv_event_t *e)
{
		//lv_task_handler();
		//tp_dev.scan(0);
    lv_obj_t * obj=lv_event_get_target(e);
    //lv_obj_t* scr=lv_event_get_user_data(e);
	if(obj==sw)                //某个元件
	{
    if(lv_obj_has_state(obj, LV_STATE_CHECKED)==0)
    {
       HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
       printf("sw\n");
		//	flag=1;
       //lv_obj_t* rec=lv_obj_create(win1_contanier);
       //lv_obj_set_style_bg_color(rec,lv_palette_main(LV_PALETTE_BLUE),0);
    }
    else if(lv_obj_has_state(obj, LV_STATE_CHECKED)!=0)
    {
        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
    }
}
else if(obj==roller)
	{
	    int index=lv_roller_get_selected(roller);
	     if(index==0)                                     //用于分波形
        {
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)sins, 100, DAC_ALIGN_12B_R); 
							/*			for(int i=0;i<100;i++)
					{
						printf("%d\n",sins[i]);
					}*/
        }
        else if(index==1)
        {
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
						HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)fangbos, 100, DAC_ALIGN_12B_R);
					for(int i=0;i<100;i++)
{
	printf("%d---%d\n",fangbos[i],adcs[i]);
}
					/*	for(int i=0;i<100;i++)
					{
						printf("%d\n",fangbos[i]);
					}*/
        }
        else if(index==2)
        {
					HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
           HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)sanjiaobosu, 100, DAC_ALIGN_12B_R);
         						/*for(int i=0;i<100;i++)
					{
						printf("%d\n",sanjiaobosu[i]);
					}*/
        }

	}
	else if(obj==slider)                //用于调波频率
    {
            //printf("slider");
           int value=lv_slider_get_value(slider);
           char buf[8];
           printf("%d\n",value);
               lv_snprintf(buf, sizeof(buf), "%dHz", value);         //将其他类型转为char类型
    lv_label_set_text(label4, buf);
    lv_obj_align_to(label4, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
			TIM2->ARR=(840000/value)-1;
			printf("ARR:%d\n",TIM2->ARR);
    }
		else if(obj==slider_time)
		{
			TIM8->ARR=lv_slider_get_value(slider_time);
			printf("slider_time:%d\n",TIM8->ARR);
		}
		else if(obj==btn_time)
		{
			dflag1=1;
			flag=1;
			frql1=lv_line_create(win2_contanier);
		static lv_style_t style_frql1;
	lv_style_init(&style_frql1);
		lv_style_set_line_width(&style_frql1,1);
		lv_style_set_line_color(&style_frql1,lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_frql1,true);
		lv_obj_set_pos(frql1,20,0);
	lv_obj_add_style(frql1,&style_frql1,0);
		lv_line_set_points(frql1,frql1s_th,2);
			lv_obj_set_style_bg_color(btn_stop,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_run,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time,lv_palette_main(LV_PALETTE_GREEN),0);
			lv_obj_set_style_bg_color(btn_time2,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_trigger,lv_palette_main(LV_PALETTE_BLUE),0);
		}
		else if(obj==btn_time2)
		{
			dflag2=1;
			flag=3;
			frql2=lv_line_create(win2_contanier);
		static lv_style_t style_frql2;
	lv_style_init(&style_frql2);
		lv_style_set_line_width(&style_frql2,1);
		lv_style_set_line_color(&style_frql2,lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_frql2,true);
		lv_obj_set_pos(frql2,20,0);
	lv_obj_add_style(frql2,&style_frql2,0);
		lv_line_set_points(frql2,frql2s_th,2);
			lv_obj_set_style_bg_color(btn_time2,lv_palette_main(LV_PALETTE_GREEN),0);
			lv_obj_set_style_bg_color(btn_run,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_stop,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_trigger,lv_palette_main(LV_PALETTE_BLUE),0);
		}
		else if(obj==btn_trigger)
		{
			dflag3=1;
			flag=3;
			trgl=lv_line_create(win2_contanier);
			static lv_style_t style_trgl;
			lv_style_init(&style_trgl);
				lv_style_set_line_width(&style_trgl,1);
		lv_style_set_line_color(&style_trgl,lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_line_rounded(&style_trgl,true);
		lv_obj_add_style(trgl,&style_trgl,0);
		lv_line_set_points(trgl,trgls_th,2);
			lv_obj_set_style_bg_color(btn_trigger,lv_palette_main(LV_PALETTE_GREEN),0);
			lv_obj_set_style_bg_color(btn_time2,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_run,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_stop,lv_palette_main(LV_PALETTE_BLUE),0);
		}
		else if(obj==btn_run)
		{
			
				int d=0;
			if(frql1s_th[0].x>=frql2s_th[0].x)
			{
				d=frql1s_th[0].x-frql2s_th[0].x;
			}else 
			if(frql1s_th[0].x<frql2s_th[0].x)
			{
				d=frql2s_th[0].x-frql1s_th[0].x;
			}
			float f=0;
			f=123000000/(d*0.738*TIM8->ARR);
			char cf[15]={0};
			sprintf(cf," f:  %.1fHz",f);
			lv_label_set_text(label_frq,cf);
			//printf("f=%f\n",f);
			flag=0;
			if(dflag1==1)
			{
				dflag1=0;
				lv_obj_del(frql1);
			}
			if(dflag2==1)
			{
				dflag2=0;
				lv_obj_del(frql2);
			}
			/*if(dflag3==1)
			{
				dflag3=0;
				lv_obj_del(trgl);
			}*/
			lv_obj_set_style_bg_color(btn_run,lv_palette_main(LV_PALETTE_YELLOW),0);
			lv_obj_set_style_bg_opa(btn_run,220,0);
			lv_obj_set_style_bg_color(btn_stop,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time2,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_trigger,lv_palette_main(LV_PALETTE_BLUE),0);
		}
		else if(obj==btn_stop)
		{
				lv_obj_set_style_bg_color(btn_stop,lv_palette_main(LV_PALETTE_RED),0);
			lv_obj_set_style_bg_color(btn_run,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_time2,lv_palette_main(LV_PALETTE_BLUE),0);
			lv_obj_set_style_bg_color(btn_trigger,lv_palette_main(LV_PALETTE_BLUE),0);
			flag=2;
		}
		else if(obj==btn_left1)
		{
			flag=3;
		frql1s_th[0].x-=2;
			frql1s_th[1].x-=2;
			lv_line_set_points(frql1,frql1s_th,2);
		}
			else if(obj==btn_right1)
		{
			flag=3;
		frql1s_th[0].x+=2;
			frql1s_th[1].x+=2;
			lv_line_set_points(frql1,frql1s_th,2);
		}
			else if(obj==btn_left2)
		{
			flag=3;
		frql2s_th[0].x-=2;
			frql2s_th[1].x-=2;
			//lv_obj_del(frql2);
			lv_line_set_points(frql2,frql2s_th,2);
		}
			else if(obj==btn_right2)
		{
			flag=3;
		frql2s_th[0].x+=2;
			frql2s_th[1].x+=2;
			//lv_obj_del(frql2);
			lv_line_set_points(frql2,frql2s_th,2);
		}
		else if(obj==btn_up)
		{
			flag=3;
		trgls_th[0].y-=2;
			trgls_th[1].y-=2;
			//lv_obj_del(frql2);
			lv_line_set_points(trgl,trgls_th,2);
		}
		else if(obj==btn_down)
		{
			flag=3;
		trgls_th[0].y+=2;
			trgls_th[1].y+=2;
			//lv_obj_del(frql2);
			lv_line_set_points(trgl,trgls_th,2);
		}
}

static void p2_event_cb(lv_event_t* event)
{
	//	lv_task_handler();
		//tp_dev.scan(0);
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
        // 给窗口1的容器添加隐藏属性，清除窗口2的隐藏属性
        lv_obj_add_flag(win1_contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
			lv_disp_set_rotation(NULL,LV_DISP_ROT_90 );   //这句让触摸部分变横屏
			LCD_Display_Dir(1);                         //这句可让画面变为横屏，但触摸部分仍按竖屏，且显示不完全

    }
}

static void p1_event_cb(lv_event_t* event)
{
		
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
        // 给窗口2的容器添加隐藏属性，清除窗口1的隐藏属性
        lv_obj_add_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win1_contanier, LV_OBJ_FLAG_HIDDEN);
			lv_disp_set_rotation(NULL,0 );
			LCD_Display_Dir(0);
    }
}

void show_win1()
{
	lv_task_handler();
		tp_dev.scan(0);
     LV_FONT_DECLARE(thuser_hanzi_font);
win1_contanier=lv_obj_create(lv_scr_act());
//static lv_style_t style1;
//lv_style_init(&style1);
//lv_obj_set_style_opa(&style1,0,0);
lv_obj_set_size(win1_contanier,240,320);
//lv_obj_add_style(win1_contanier,&style1,0);
/*lv_obj_t *obj=lv_obj_create(scr);
lv_obj_set_pos(obj,120,160);
lv_obj_set_size(obj,40,40);*/
 sw= lv_switch_create(win1_contanier);
  lv_obj_set_pos(sw,44,1);
  lv_obj_t* label=lv_label_create(win1_contanier);
lv_obj_set_pos(label,0,0);
  //lv_obj_add_event_cb()
  lv_obj_set_style_text_font(label,&thuser_hanzi_font,0);

//lv_obj_set_size(label,50,50);
//lv_obj_set_pos(label,3,7);
lv_label_set_text(label,"LED");

//lv_obj_clear_state(sw,LV_STATE_DISABLED);
lv_obj_add_state(sw,LV_STATE_CHECKED);
lv_obj_add_event_cb(sw,my_event,LV_EVENT_VALUE_CHANGED,NULL);

roller=lv_roller_create(win1_contanier);
lv_obj_set_pos(roller,70,80);
lv_obj_set_size(roller,80,100);
lv_obj_set_style_text_font(roller,&thuser_hanzi_font,0);
lv_roller_set_options(roller,"正弦波\n方波\n三角波" , LV_ROLLER_MODE_NORMAL);
lv_obj_add_event_cb(roller,my_event,LV_EVENT_VALUE_CHANGED,NULL);

  lv_obj_t* label1=lv_label_create(win1_contanier);
   lv_obj_align_to(label1,roller,LV_ALIGN_OUT_TOP_MID,-4,-16);
   lv_obj_set_style_text_font(label1,&thuser_hanzi_font,0);
   lv_label_set_text(label1,"类型");

slider=lv_slider_create(win1_contanier);

lv_obj_set_size(slider,150,10);
lv_obj_align_to(slider,roller,LV_ALIGN_OUT_BOTTOM_MID,4,60);
lv_slider_set_range(slider,100,5000);
lv_obj_add_event_cb(slider,my_event,LV_EVENT_VALUE_CHANGED,NULL);

 lv_obj_t* label2=lv_label_create(win1_contanier);
   lv_obj_align_to(label2,slider,LV_ALIGN_OUT_TOP_MID,-9,-20);
   lv_obj_set_style_text_font(label2,&thuser_hanzi_font,0);
   lv_label_set_text(label2,"频率");

     label4 = lv_label_create(win1_contanier);
    lv_label_set_text(label4, "100Hz");
lv_obj_align_to(label4, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

   lv_obj_t* btn=lv_btn_create(win1_contanier);
   lv_obj_set_pos(btn,150,8);
   lv_obj_set_size(btn,60,36);
   lv_obj_t* label3=lv_label_create(btn);
   lv_obj_center(label3);
   lv_label_set_text(label3,"P2");

	 lv_obj_add_event_cb(btn,p2_event_cb,LV_EVENT_CLICKED,NULL);

//lv_task_handler();
	//	tp_dev.scan(0);
}

void show_win2()
{
	//printf("th02");
	//lv_task_handler();
	//	tp_dev.scan(0);

	
	win2_contanier=lv_obj_create(lv_scr_act());
lv_obj_set_style_bg_color(win2_contanier,lv_color_hex(0x00000000),0);
 //lv_style_t style2;
//lv_style_init(&style2);
//lv_obj_set_style_opa(win2_contanier,100,0);
 //sw= lv_switch_create(win2_contanier);
  //lv_obj_set_pos(sw,50,8);
  //lv_obj_set_style_bg_color(win2_contanier, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_size(win2_contanier, 320, 240);
        //lv_obj_add_style(win2_contanier, &style2, 0);
         lv_obj_t* btn=lv_btn_create(win2_contanier);
   lv_obj_set_pos(btn,-10,-10);
   lv_obj_set_size(btn,30,18);
   lv_obj_t* label3=lv_label_create(btn);
   lv_obj_center(label3);
   lv_label_set_text(label3,"P1");

	 lv_obj_add_event_cb(btn,p1_event_cb,LV_EVENT_CLICKED,NULL);


			
		
	static lv_style_t style_line;
	lv_style_init(&style_line);
		lv_style_set_line_width(&style_line,2);
		lv_style_set_line_color(&style_line,lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_line_rounded(&style_line,true);

    line= lv_line_create(win2_contanier);
		lv_obj_set_pos(line,20,0);
	lv_obj_add_style(line,&style_line,0);
	
	static lv_style_t vpp_style;                                           //显示电压
	lv_style_init(&vpp_style);
	 label_vpp=lv_label_create(win2_contanier);
	lv_obj_set_pos(label_vpp,143,200);
	lv_obj_set_size(label_vpp,110,20);
    lv_obj_set_style_bg_color(label_vpp,lv_palette_main(LV_PALETTE_YELLOW),0);
    lv_obj_set_style_bg_opa(label_vpp,255,0);
    lv_style_set_radius(&vpp_style,6);
    lv_obj_add_style(label_vpp,&vpp_style,0);
		
		static lv_style_t vmax_style;                                           //显示最大电压
	lv_style_init(&vmax_style);
	 label_vmax=lv_label_create(win2_contanier);
	lv_obj_set_pos(label_vmax,20,175);
	lv_obj_set_size(label_vmax,120,20);
    lv_obj_set_style_bg_color(label_vmax,lv_palette_main(LV_PALETTE_PINK),0);
    lv_obj_set_style_bg_opa(label_vmax,255,0);
    lv_style_set_radius(&vmax_style,6);
    lv_obj_add_style(label_vmax,&vmax_style,0);
		
		static lv_style_t vmin_style;                                           //显示最小电压
	lv_style_init(&vmin_style);
	 label_vmin=lv_label_create(win2_contanier);
	lv_obj_set_pos(label_vmin,20,200);
	lv_obj_set_size(label_vmin,120,20);
    lv_obj_set_style_bg_color(label_vmin,lv_palette_main(LV_PALETTE_PINK),0);
    lv_obj_set_style_bg_opa(label_vmin,255,0);
    lv_style_set_radius(&vmin_style,6);
    lv_obj_add_style(label_vmin,&vmin_style,0);
		
		static lv_style_t frq_style;                                      //显示频率
	lv_style_init(&frq_style);
	label_frq=lv_label_create(win2_contanier);
	lv_label_set_text(label_frq," f:  0");
	lv_obj_set_pos(label_frq,256,200);
	lv_obj_set_size(label_frq,100,20);
    lv_obj_set_style_bg_color(label_frq,lv_palette_main(LV_PALETTE_GREEN),0);
    lv_obj_set_style_bg_opa(label_frq,255,0);
    lv_style_set_radius(&frq_style,6);
    lv_obj_add_style(label_frq,&frq_style,0);
		
		static lv_style_t timesld_style;                                     
	lv_style_init(&timesld_style);
	label_timesld=lv_label_create(win2_contanier);
	lv_label_set_text(label_timesld," T-scale");
	lv_obj_set_size(label_timesld,70,16);
	lv_obj_set_pos(label_timesld,460,-10);
    lv_obj_set_style_bg_color(label_timesld,lv_palette_main(LV_PALETTE_TEAL),0);
		lv_obj_set_style_bg_opa(label_timesld,255,0);
    lv_style_set_radius(&timesld_style,6);
    lv_obj_add_style(label_timesld,&timesld_style,0);                          //修改
	//	lv_obj_align_to(label_timesld,slider_time,LV_ALIGN_OUT_TOP_MID,0,0);
		
		slider_time=lv_slider_create(win2_contanier);

lv_obj_set_size(slider_time,10,200);
lv_obj_set_pos(slider_time,490,16);
lv_slider_set_range(slider_time,123,20000);
lv_obj_add_event_cb(slider_time,my_event,LV_EVENT_VALUE_CHANGED,NULL);

btn_time=lv_btn_create(win2_contanier);
lv_obj_set_pos(btn_time,390,10);
lv_obj_set_size(btn_time,60,40);
lv_obj_add_event_cb(btn_time,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_time=lv_label_create(btn_time);
lv_label_set_text(label_time,"frq1");

btn_time2=lv_btn_create(win2_contanier);
lv_obj_set_pos(btn_time2,390,160);
lv_obj_set_size(btn_time2,60,40);
lv_obj_add_event_cb(btn_time2,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_time2=lv_label_create(btn_time2);
lv_label_set_text(label_time2,"frq2");

btn_trigger=lv_btn_create(win2_contanier);
lv_obj_set_pos(btn_trigger,-10,80);
lv_obj_set_size(btn_trigger,24,60);
lv_obj_add_event_cb(btn_trigger,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_trg=lv_label_create(btn_trigger);
lv_label_set_text(label_trg,"T\nr\ng");
lv_obj_align_to(label_trg,btn_trigger,LV_ALIGN_CENTER,0,0);
lv_label_set_long_mode(label_trg,LV_LABEL_LONG_WRAP);

btn_run=lv_btn_create(win2_contanier);
lv_obj_set_pos(btn_run,390,60);
lv_obj_set_size(btn_run,60,40);
lv_obj_add_event_cb(btn_run,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_run=lv_label_create(btn_run);
lv_label_set_text(label_run,"RUN");

btn_stop=lv_btn_create(win2_contanier);
lv_obj_set_pos(btn_stop,390,110);
lv_obj_set_size(btn_stop,60,40);
lv_obj_add_event_cb(btn_stop,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_stop=lv_label_create(btn_stop);
lv_label_set_text(label_stop,"STOP");



btn_left1=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_left1,btn_time,LV_ALIGN_OUT_LEFT_MID,0,0);
lv_obj_set_size(btn_left1,20,20);
lv_obj_add_event_cb(btn_left1,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_left1=lv_label_create(btn_left1);
lv_label_set_text(label_left1,"-2");
lv_obj_align_to(label_left1,btn_left1,LV_ALIGN_CENTER,0,0);

btn_right1=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_right1,btn_time,LV_ALIGN_OUT_RIGHT_MID,5,0);
lv_obj_set_size(btn_right1,20,20);
lv_obj_add_event_cb(btn_right1,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_right1=lv_label_create(btn_right1);
lv_label_set_text(label_right1,"+2");
lv_obj_align_to(label_right1,btn_right1,LV_ALIGN_CENTER,0,0);

btn_left2=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_left2,btn_time2,LV_ALIGN_OUT_LEFT_MID,0,0);
lv_obj_set_size(btn_left2,20,20);
lv_obj_add_event_cb(btn_left2,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_left2=lv_label_create(btn_left2);
lv_label_set_text(label_left2,"-2");
lv_obj_align_to(label_left2,btn_left2,LV_ALIGN_CENTER,0,0);

btn_right2=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_right2,btn_time2,LV_ALIGN_OUT_RIGHT_MID,5,0);
lv_obj_set_size(btn_right2,20,20);
lv_obj_add_event_cb(btn_right2,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_right2=lv_label_create(btn_right2);
lv_label_set_text(label_right2,"+2");
lv_obj_align_to(label_right2,btn_right2,LV_ALIGN_CENTER,0,0);

btn_up=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_up,btn_trigger,LV_ALIGN_OUT_TOP_MID,3,-8);
lv_obj_set_size(btn_up,20,20);
lv_obj_add_event_cb(btn_up,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_up=lv_label_create(btn_up);
lv_label_set_text(label_up,"+2");
lv_obj_align_to(label_up,btn_up,LV_ALIGN_CENTER,0,0);

btn_down=lv_btn_create(win2_contanier);
lv_obj_align_to(btn_down,btn_trigger,LV_ALIGN_OUT_BOTTOM_MID,3,5);
lv_obj_set_size(btn_down,20,20);
lv_obj_add_event_cb(btn_down,my_event,LV_EVENT_CLICKED,NULL);
lv_obj_t* label_down=lv_label_create(btn_down);
lv_label_set_text(label_down,"-2");
lv_obj_align_to(label_down,btn_down,LV_ALIGN_CENTER,0,0);

lv_obj_t*cenl=lv_line_create(win2_contanier);
			static lv_style_t style_cenl;
			lv_style_init(&style_cenl);
				lv_style_set_line_width(&style_cenl,1);
		lv_style_set_line_color(&style_cenl,lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_cenl,true);
		lv_obj_add_style(cenl,&style_cenl,0);
		lv_line_set_points(cenl,cenls_th,2);
		
lv_timer_t* timer;
	timer=lv_timer_create(shibo,50,NULL);
/*while(1)
{
	if(flag==0)
	{
	//printf("th03");
	lv_task_handler();                 //time8:1/42,000,000*（3+12.5）=0.369μs采集一次，最高采样频率约2710khz
		tp_dev.scan(0);                        
	HAL_TIM_Base_Start_IT(&htim8);
	HAL_ADC_Start_DMA(&hadc1,adcs,320);           //1/21,000,000*（3+12.5）=0.738μs采集一次，最高采样频率约1355khz
		                                                                      //0.236ms采集320次
				for(ms=0;ms<320;ms++)
			{
				tempth[ms] = adcs[ms]*(3.3/4096)*50;               
			}
			int max=tempth[0];
				int min=tempth[0];
			for(ms=0;ms<320;ms++)
				{
					if(tempth[ms]>max)
					{
						max=tempth[ms];
					}
					if(tempth[ms]<min)
					{
						min=tempth[ms];
					}
				}
			HAL_Delay(20);
				HAL_ADC_Stop_DMA(&hadc1);
			if( ms == 320)                            
				{                                                                     
				for(ms=0;ms<320;ms++)
				{
					xemp[ms]=tempth[ms];
				}
				
				vcc=(max-min)*20;
				//sprintf(vccs,"%.2f",vcc);
				lv_snprintf(vccs, sizeof(vccs), "VPP:%dmV", vcc);         
    lv_label_set_text(label_vpp, vccs);
    
				for(ms=0;ms<319;ms++)
				{
				lines_th[ms].x=ms;
					lines_th[ms].y=xemp[ms];
				lv_line_set_points(line,lines_th,ms);
				}
					ms = 0;
		}
				//HAL_Delay(500);
	}		else if(flag==1)
	{
		printf("flag=1");
	}
}*/


}
 void my_printf()
 {
	 printf("ka");
 }
 void shibo()
 {

	if(flag==0)     //RUN
	{
	
	//printf("th03");
	//lv_task_handler();
	//	tp_dev.scan(0);
	HAL_TIM_Base_Start_IT(&htim8);											//开启定时器8的中断，进行ADC的采集
				HAL_ADC_Start_DMA(&hadc1,adcs,320);           //1/21,000,000*（3+12.5）=0.738μs采集一次
		                                                                      //0.236ms采集320次
		for(int t=0;t<320;t++)
		printf("%d\n",adcs[t]);
				for(ms=0;ms<320;ms++)
			{
				tempth[ms] = adcs[ms]*(3.3/4096)*50;         //*50是为了将实际的电压值等比转换为屏幕上的坐标方便画图
			}
			int max=tempth[0];
				int min=tempth[0];
			for(ms=0;ms<320;ms++)
				{
					if(tempth[ms]>max)
					{
						max=tempth[ms];
					}
					if(tempth[ms]<min)
					{
						min=tempth[ms];
					}
				}
			HAL_Delay(20);
				HAL_ADC_Stop_DMA(&hadc1);
				if(170-tempth[160]<=trgls_th[0].y+5&&170-tempth[160]>=trgls_th[0].y-5&&tempth[165]>=tempth[155])//&&tempth[165]>=tempth[155]
					{   
			if( ms == 320)                            
				{                                                                     
				for(ms=0;ms<320;ms++)
				{
					xemp[ms]=tempth[ms];
				}
				
				vcc=(max-min)*20;
				//sprintf(vccs,"%.2f",vcc);
				lv_snprintf(vccs, sizeof(vccs), " VPP:%dmV", vcc);
lv_snprintf(vmaxs, sizeof(vmaxs), " Vmax:%dmV", max*20);
lv_snprintf(vmins, sizeof(vmins), " Vmin:%dmV", min*20);				
    lv_label_set_text(label_vpp, vccs);
    lv_label_set_text(label_vmax, vmaxs);
				lv_label_set_text(label_vmin, vmins);
				for(ms=0;ms<319;ms++)
				{
				lines_th[ms].x=ms;
					lines_th[ms].y=170-xemp[ms];
				lv_line_set_points(line,lines_th,ms);
				}
					ms = 0;
		}
				//HAL_Delay(500);
	} else if(170-max>trgls_th[0].y||170-min<trgls_th[0].y)
					{
						if( ms == 320)                            
				{                                                                     
				for(ms=0;ms<320;ms++)
				{
					xemp[ms]=tempth[ms];
				}
				
				vcc=(max-min)*20;
				//sprintf(vccs,"%.2f",vcc);
				lv_snprintf(vccs, sizeof(vccs), " VPP:%dmV", vcc);
lv_snprintf(vmaxs, sizeof(vmaxs), " Vmax:%dmV", max*20);
lv_snprintf(vmins, sizeof(vmins), " Vmin:%dmV", min*20);				
    lv_label_set_text(label_vpp, vccs);
    lv_label_set_text(label_vmax, vmaxs);
				lv_label_set_text(label_vmin, vmins);
				for(ms=0;ms<319;ms++)
				{
				lines_th[ms].x=ms;
					lines_th[ms].y=170-xemp[ms];
				lv_line_set_points(line,lines_th,ms);
				}
					ms = 0;
		}
					}
}
	else if(flag==1)  //frq
	{
		//printf("th04");
		//lv_obj_clear_flag(frql1, LV_OBJ_FLAG_HIDDEN);
		
	}
	else if(flag==2)   //STOP
	{
	//	printf("th05");
		HAL_TIM_Base_Stop_IT(&htim8);         
	}
}
 
void test_start()
{
		//lv_task_handler();
		//tp_dev.scan(0);
	//printf("th01");
	flag=0;

		 show_win1();
    show_win2();
	 
	lv_obj_add_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
    // 给窗口2的容器添加隐藏属性...
    
}

