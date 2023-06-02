#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/web_configuator/base/web_configurator_diction_base.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 9   
#define PIN_GPIO_OF_CONFIG_BUTTON_AT_STANDARD -1


class WebConfiguratorDiction_Standard:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_Standard(){};
		void Init(){
			_Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
		    Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON_AT_STANDARD;
    		HtmlFilename_of_Configurator = __html_file;
		}
	
	private:
		WebConnfigurator_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		const char* __html_file="/serial_port_sniffer.html";
};