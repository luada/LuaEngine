--[[Writen by Luada

�¼������ӿ�

--]]

require "base.interface"


IEventListener = interface(nil, 
			"getPriority",		---getPriority(),����ֵ����uint�������ߵ�����Ȩ������˳�򣺰�����Ȩ�Ӵ�С
			"getName",			---getName(),����ֵ����string
			"onEvent"			---onEvent(eventKey, ...)������ֵ����boolean����Ϊtrueʱ�����¼��������´���
			)
