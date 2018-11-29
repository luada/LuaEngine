--[[Created by Luada
��Ϊ�ڵ㣺��Ϊ����Ҷ�ڵ㣬������߼�����������ȥ�̳к�ʵ��
--]]

ActionBevNode = class(BevNode)

function ActionBevNode:__init(name, precondition)
	BevNode.__init(self, name, precondition)
	self._status = ActionStatus.Ready
	self._needExit = false
end

function ActionBevNode:__release()
end

function ActionBevNode:onTransition_(input)
	--[[
	�ڵ�ת��ʱ�Ļص�����
	@param	input��
	@type	input��any
	--]]
	if self._needExit then
		self:onExit_(input, BevRunningStatus.ErrorTransition)
	end
	
	self:setActiveNode(nil)
	self._status = ActionStatus.Ready
	self._needExit = false
end

function ActionBevNode:onTick_(input, output)
	--[[
	��Ϊ���Ĵ���ص�����
	@param	input��
	@type	input��any
	@param	output��
	@type	output��object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	local excStatus = BevRunningStatus.Finish
	if self._status == ActionStatus.Ready then
		self:onEnter_(input)
		self._needExit = true
		self._status = ActionStatus.Running
		self:setActiveNode(self)
	end
	if self._status == ActionStatus.Running then
		excStatus = self:onExecute_(input, output)
		self:setActiveNode(self)
		if excStatus ~= BevRunningStatus.Executing then
			self._status = ActionStatus.Finish
		end
	end
	if self._status == ActionStatus.Finish then
		if self._needExit then
			self:onExit_(input, excStatus)
		end
		self._status = ActionStatus.Ready
		self._needExit = false
		self:setActiveNode(nil)
	end
	return excStatus
end

function ActionBevNode:onEnter_(input)
	--[[
	��Ϊ�ڵ����ʱ�Ļص�������������״̬��ʼ�����������������̳�ʵ��
	@param	input��
	@type	input��any
	--]] 
end

function ActionBevNode:onExit_(input, exitStatus)
	--[[
	��Ϊ�ڵ����ʱ�Ļص�������������״̬�����������������̳�ʵ��
	@param	input��
	@type	input��any
	@param	exitStatus��
	@type	exitStatus��BevRunningStatus
	--]]
end

function ActionBevNode:onExecute_(input, output)
	--[[
	��Ϊ�ڵ�ִ�лص��������������������̳�ʵ��
	@param	input:
	@type	input: any
	@param	output:
	@type	output: object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	return BevRunningStatus.Finish
end
