--[[Writen by Luada

事件监听接口

--]]

require "base.interface"


IEventListener = interface(nil, 
			"getPriority",		---getPriority(),返回值类型uint；监听者的优先权；触发顺序：按优先权从大到小
			"getName",			---getName(),返回值类型string
			"onEvent"			---onEvent(eventKey, ...)，返回值类型boolean；仅为true时本组事件继续往下触发
			)
