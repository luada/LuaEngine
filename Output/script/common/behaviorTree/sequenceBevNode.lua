--[[Created by Luada
行为树的序列节点

测试阶段：
	若是从头开始的，则调用第一个子节点的test方法，将其返回值作为自身的返回值返回。
	否则，调用当前运行节点的test方法，将其返回值作为自身的返回值返回。

处理阶段：
	调用可以运行的子节点的tick方法，若返回运行结束，则将下一个子节点作为当前运行节点，
	若当前已是最后一个子节点，表示该序列已经运行结束，则自身返回运行结束。若子节点返
	回运行中，则用它所返回的运行状态作为自身的运行状态返回
--]]

SequenceBevNode = class(ComponentBevNode)

function SequenceBevNode:__init(name)
	ComponentBevNode.__init(self, name)
	self._curIdx = -1
end

function SequenceBevNode:__release()
end

function SequenceBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	local testIdx = self._curIdx == -1 and 1 or self._curIdx
	if self:checkIndex_(testIdx) then
		if self.childNodes_[testIdx]:test(input) then
			return true
		end
	end
	return false
end

function SequenceBevNode:onTransition_(input)
	--[[
	节点转移时的回调处理
	@param	input：
	@type	input：any
	--]]
	if self:checkIndex_(self._curIdx) then
		self.childNodes_[self._curIdx]:transition(input)
	end
	self._curIdx = -1
end

function SequenceBevNode:onTick_(input, output)
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
	local curIdx = self._curIdx == -1 and 1 or self._curIdx
	if self:checkIndex_(curIdx) then
		excStatus = self.childNodes_[curIdx]:tick(input, output)
		if excStatus == BevRunningStatus.Finish then
			self._curIdx = curIdx + 1
			if self._curIdx >= #self.childNodes_ then
				self._curIdx = -1
			else
				excStatus = BevRunningStatus.Executing
			end
		elseif excStatus < 0 then	---error occurred
			self._curIdx = -1
		end
	end
	return excStatus
end
