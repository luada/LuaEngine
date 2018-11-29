--[[Created by Luada
行为节点：行为树的叶节点，具体的逻辑处理由子类去继承和实现
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
	节点转移时的回调处理
	@param	input：
	@type	input：any
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
	行为树的处理回调函数
	@param	input：
	@type	input：any
	@param	output：
	@type	output：object or table
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
	行为节点进入时的回调函数，可用于状态初始化；具体操作由子类继承实现
	@param	input：
	@type	input：any
	--]] 
end

function ActionBevNode:onExit_(input, exitStatus)
	--[[
	行为节点进入时的回调函数，可用于状态清理；具体操作由子类继承实现
	@param	input：
	@type	input：any
	@param	exitStatus：
	@type	exitStatus：BevRunningStatus
	--]]
end

function ActionBevNode:onExecute_(input, output)
	--[[
	行为节点执行回调函数；具体操作由子类继承实现
	@param	input:
	@type	input: any
	@param	output:
	@type	output: object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	return BevRunningStatus.Finish
end
