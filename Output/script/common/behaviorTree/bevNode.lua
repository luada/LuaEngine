--[[Created by Luada
行为树的基类

在行为树上有两大类的节点，一种我称之为“控制节点”，像“选择节点”，“并行节点”，
“序列节点”都属于此类，这类节点负责行为树逻辑的控制，是和具体的游戏逻辑无关的，
属于行为树库的一部分，并且这类节点一般不会作为叶节点。还有一类称为“行为节点”，
也就是行为树上挂载的具体行为，是和游戏逻辑相关的，不属于行为树库的一部分，需要
自己去继承和实现，这类节点一般都作为叶节点出现。

--]]

require "common.behaviorTree.const"
require "common.behaviorTree.bevSearchTracer"

BevNode = class()

function BevNode:__init(name, precondition)
	--[[
	行为树初始化
	@param	name: 名字；为nil时，命名为"UNNAMED"
	@type	name: string
	@param	precondition: 外部的前提条件
	@type	precondition：object of BevPrecondition or nil
	--]]
	self._objName = name == nil and "UNNAMED" or name
	self.activeNode_ = nil
	self.lastActiveNode_ = nil
	self.parent_ = nil
	self._precondition = precondition
	self.childNodes_ = {}
end

function BevNode:__release()
	for _, v in ipairs(self.childNodes_) do
		v:release()
	end
	self.childNodes_ = nil
	self:setPrecondition(nil)
end

function BevNode:test(input)
	--[[
	行为树的测试函数，如果有外部前提条件时，先测试外部前提条件
	@param	input：输入参数
	@type	input：any
	@param	return: 
	@type	return: boolean
	--]]
	return (not self._precondition or self._precondition:test(input)) and self:onTest_(input)
end

function BevNode:tick(input, output)
	--[[
	行为树的处理函数
	@param	input：
	@type	input：any
	@type	output：
	@type	output：object or table
	@param	return：
	@type	return：BevRunningStatus
	--]]
	return self:onTick_(input, output)
end

function BevNode:load(...)
	--[[
	加载配置数据
	@param	arg：
	@type	arg：Args
	--]]
	self:onLoad_(...)
end

function BevNode:setParent(parent, withAddChild)
	--[[
	绑定父节点
	@param	parent: 父节点
	@type	parent: object of BevNode
	@param	withAddChild: 是否向父节点添加自己作为其孩子
	@type	withAddChild：boolean
	--]]
	self.parent_ = parent
	if withAddChild then
		self.parent_:addChildNode(self)
	end
end

function BevNode:addChildNode(child)
	--[[
	添加孩子节点
	@param	child：
	@type	child：object of BevNode
	@param	return：返回本身，便于连续调用对象方法
	@type	return：self
	--]]
	table.insert(self.childNodes_, child)
	return self
end

function BevNode:getLastActiveNode()
	--[[
	获取上一活动的节点
	@param	return：
	@type	return：object of BevNode or nil
	--]]
	return self.lastActiveNode_
end

function BevNode:setActiveNode(node)
	--[[
	设置活动节点；该方法会向父节点设置活动节点
	@param	node：
	@type	node：object of BevNode or nil
	--]]
	if node ~= nil then
		DEBUG_MSG("setActiveNode", node:getName(), "--->", self:getName())
	end
	self.lastActiveNode_ = self.activeNode_
	self.activeNode_ = node
	if self.parent_ ~= nil then
		self.parent_:setActiveNode(node)
	end
end

function BevNode:transition(input)
	--[[
	节点转移时的处理
	@param	input：
	@type	input：any
	@param	return：
	@type	return：BevRunningStatus
	--]]
	return self:onTransition_(input)
end

function BevNode:getName()
	--[[
	获取名字；通常用在调试行为树上
	@param	return：
	@type	return：string
	--]]
	return self._objName
end

function BevNode:setPrecondition(precondition)
	--[[
	设置行为树的外部前提条件;该方法会释放旧的外部前提条件
	@param	precondition：
	@type	precondition：object of BevPrecondition or nil
	--]]
	if self._precondition ~= precondition then
		release(self._precondition)
		self._precondition = precondition
	end
end

function BevNode:checkIndex_(index)
	--[[
	检查下标索引是否合法
	@param	index：
	@type	index：int
	@param	return：
	@type	return：boolean
	--]]
	return index >= 1 and index <= #self.childNodes_
end

function BevNode:onLoad_(...)
	--[[
	加载配置回调函数；由子类实现
	@param	arg:
	@type	arg: Args
	--]]
end

function BevNode:onTest_(input)
	--[[
	测试函数的回调处理；由子类实现
	@param	input：
	@type	input：any
	@param	return：
	@type	return：boolean
	--]]
	return true
end

function BevNode:onTransition_(input)
	--[[
	节点转移时的回调处理；由子类实现
	@param	input：
	@type	input：any
	--]]
end

function BevNode:onTick_(input, output)
	--[[
	行为树的处理回调函数；由子类实现
	@param	input：
	@type	input：any
	@param	output：
	@type	output：object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	return BevRunningStatus.Finish
end
