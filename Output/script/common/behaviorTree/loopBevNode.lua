--[[Created by Luada
行为树的循环节点

测试阶段：
	预设的循环次数到了就返回false，否则，只调用第一个子节点的test方法，
	用它所返回的值作为自身的值返回
	
处理阶段：
	只调用第一个节点的tick方法，若返回运行结束，则看是否需要重复运行，
	若循环次数没到，则自身返回运行中，若循环次数已到，则返回运行结束

--]]

LoopBevNode = class(ComponentBevNode)

function LoopBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._loopCount = loopCount		---为-1时，表示无限次调用
	self._currentCount = 0
end

function LoopBevNode:__release()
end

function LoopBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	local check = self._loopCount == -1 or self._currentCount < self._loopCount
	if not check then
		return false
	end
	
	if self:checkIndex_(1) then
		return self.childNodes_[1]:test(input)
	end
	
	return false
end

function LoopBevNode:onTransition_(input)
	--[[
	节点转移时的回调处理
	@param	input：
	@type	input：any
	--]]
	if self:checkIndex_(1) then
		self.childNodes_[1]:transition(input)
	end
	self._currentCount = 0
end

function LoopBevNode:onTick_(input, output)
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
	if self:checkIndex_(1) then
		excStatus = self.childNodes_[1]:tick(input, output)
	end
	if excStatus == BevRunningStatus.Finish then
		if self._loopCount ~= -1 then
			self._currentCount = self._currentCount + 1
			if self._currentCount ~= self._loopCount then
				excStatus = BevRunningStatus.Executing
			end
		else
			excStatus = BevRunningStatus.Executing
		end
	end
	
	if excStatus ~= BevRunningStatus.Executing then
		self._currentCount = 0
	end
	return excStatus
end