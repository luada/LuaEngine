--[[Writen by Luada

�¼�����ģ��

--]]

require "base.class"

--------------------------------------------------------------------------------
local Event = class()

function Event:__init()
	self._listeners = {}
end

function Event:__release()
	self._listeners = nil
end

function Event:fire(eventKey, ...)
	--[[
	���������¼�
	@param	eventKey: �¼�����
	@type	eventKey: eventKey
	@param	arg: ���������Ĳ����б�
	@type	arg: ����ʵ�ʵļ�����������
	@param	return: ��һ�������ж��˴�����ʱ����false�����򷵻�true
	@type	return: boolean(true or false)
	--]]
	for _, listener in ipairs(self._listeners) do
		if not listener:onEvent(eventKey, ...) then
			return	false
		end
	end
	return true
end

function Event:addListener(listener)
	--[[
	�ڱ����¼�����Ӽ�����
	@param	listener: ��Ӧ�¼��ļ�������
	@type	listener: object
	--]]
	table.insert(self._listeners, listener)
	
	function comparator(a, b)
		return a:getPriority() > b:getPriority()
	end
	
	table.sort(self._listeners, comparator)
end

function Event:getListener(name)
	--[[
	�������֣���ȡ������ʵ��
	@param	name������������
	@type	name��string
	@param	return��������ʵ����û���ҵ�ʱ������nil
	@type	return��object of IEventListener or nil
	--]]
	for _, listener in ipairs(self._listeners) do
		if listener:getName() == name then
			return listener
		end
	end
	return nil
end

function Event:removeListener(listener)
	--[[
	��listener����ӱ����¼���ɾ��
	@param	listener: ��Ӧ�¼��ļ�������
	@type	listener: object
	--]]
	table.rmArrayValue(self._listeners, listener)
end

function Event:hasListeners()
	--[[
	�жϱ����¼����Ƿ���ڼ�������
	--]]
	return self._listeners ~= nil and #self._listeners > 0
end

--------------------------------------------------------------------------------
ECenter = class(Singleton)

function ECenter:__init()
	self._events = {}
end

function ECenter:__release()
	for _, event in pairs(self._events) do
		event:release()
	end
	self._events = nil
end

function ECenter:registerEvent(eventKey, listener)
	--[[
	ע��һ���¼�
	@param  eventKey: �¼�����
	@type	eventKey: eventType
	@param	listener: ��Ӧ�¼��ļ�������
	@type	listener: object
	--]]
	local event = self._events[eventKey]
	if event == nil then
		event = Event()
		self._events[eventKey] = event
	end
	event:addListener(listener)
end

function ECenter:deregisterEvent(eventKey, listener)
	--[[
	ע��һ���¼�
	@param  eventKey: �¼�����
	@type	eventKey: eventType
	@param	listener: ��Ӧ�¼��ļ�������
	@type	listener: object
	--]]
	local event = self._events[eventKey]
	if event ~= nil then
		event:removeListener(listener)
		if not event:hasListeners() then
			event:release()
			self._events[eventKey] = nil
		end
	end
end

function ECenter:getListener(name, eventKey)
	--[[
	�������ֺ��¼����ͣ���ȡ�����ߡ����¼����Ͳ���Ϊnilʱ�������ȫ������
	@param	name������������
	@type	name��string
	@param	eventKey: �¼����ͣ�����Ϊnil
	@type	eventKey��eventType or nil
	@param	return������ֵ������������ʵ��, table{�¼�����})��û���ҵ�ʱ������nil
	@type	return��(object of IEventListener, table{eventType}) or nil
	--]]
	if eventKey ~= nil then
		local event = self._events[eventKey]
		if event ~= nil then
			local listener = event:getListener(name)
			if listener ~= nil then
				return listener, {eventKey}
			end
		end
		return nil
	end
	
	local findListener = nil
	local findEventKeys = {}
	for eventKey, event in pairs(self._events) do
		local listener = event:getListener(name)
		if listener ~= nil then
			assert(findListener == nil or listener == findListener, 
					string.format("The name(%s) of Listener is not unique!", name))
			findListener = listener
			table.insert(findEventKeys, eventKey)
		end
	end
	
	if findListener ~= nil then
		return findListener, findEventKeys
	end
	
	return nil
end

function ECenter:fireEvent(eventKey, ...)
	--[[
	����һ���¼���ֱ����һ�������ж��˴�����
	@param	eventKey: �¼�����
	@type	eventKey: eventType
	@param	arg: ���������Ĳ����б�
	@type	arg: ����ʵ�ʵĴ�����������
	@param	return: ��һ�������ж��˴�����ʱ����false�����򷵻�true
	@type	return: boolean(true or false)
	--]]
	local event = self._events[eventKey]
	if event ~= nil then
		return event:fire(eventKey, ...)
	end
	return true
end

--------------------------------------------------------------------------------