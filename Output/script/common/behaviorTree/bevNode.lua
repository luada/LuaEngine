--[[Created by Luada
��Ϊ���Ļ���

����Ϊ������������Ľڵ㣬һ���ҳ�֮Ϊ�����ƽڵ㡱����ѡ��ڵ㡱�������нڵ㡱��
�����нڵ㡱�����ڴ��࣬����ڵ㸺����Ϊ���߼��Ŀ��ƣ��Ǻ;������Ϸ�߼��޹صģ�
������Ϊ�����һ���֣���������ڵ�һ�㲻����ΪҶ�ڵ㡣����һ���Ϊ����Ϊ�ڵ㡱��
Ҳ������Ϊ���Ϲ��صľ�����Ϊ���Ǻ���Ϸ�߼���صģ���������Ϊ�����һ���֣���Ҫ
�Լ�ȥ�̳к�ʵ�֣�����ڵ�һ�㶼��ΪҶ�ڵ���֡�

--]]

require "common.behaviorTree.const"
require "common.behaviorTree.bevSearchTracer"

BevNode = class()

function BevNode:__init(name, precondition)
	--[[
	��Ϊ����ʼ��
	@param	name: ���֣�Ϊnilʱ������Ϊ"UNNAMED"
	@type	name: string
	@param	precondition: �ⲿ��ǰ������
	@type	precondition��object of BevPrecondition or nil
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
	��Ϊ���Ĳ��Ժ�����������ⲿǰ������ʱ���Ȳ����ⲿǰ������
	@param	input���������
	@type	input��any
	@param	return: 
	@type	return: boolean
	--]]
	return (not self._precondition or self._precondition:test(input)) and self:onTest_(input)
end

function BevNode:tick(input, output)
	--[[
	��Ϊ���Ĵ�����
	@param	input��
	@type	input��any
	@type	output��
	@type	output��object or table
	@param	return��
	@type	return��BevRunningStatus
	--]]
	return self:onTick_(input, output)
end

function BevNode:load(...)
	--[[
	������������
	@param	arg��
	@type	arg��Args
	--]]
	self:onLoad_(...)
end

function BevNode:setParent(parent, withAddChild)
	--[[
	�󶨸��ڵ�
	@param	parent: ���ڵ�
	@type	parent: object of BevNode
	@param	withAddChild: �Ƿ��򸸽ڵ�����Լ���Ϊ�亢��
	@type	withAddChild��boolean
	--]]
	self.parent_ = parent
	if withAddChild then
		self.parent_:addChildNode(self)
	end
end

function BevNode:addChildNode(child)
	--[[
	��Ӻ��ӽڵ�
	@param	child��
	@type	child��object of BevNode
	@param	return�����ر��������������ö��󷽷�
	@type	return��self
	--]]
	table.insert(self.childNodes_, child)
	return self
end

function BevNode:getLastActiveNode()
	--[[
	��ȡ��һ��Ľڵ�
	@param	return��
	@type	return��object of BevNode or nil
	--]]
	return self.lastActiveNode_
end

function BevNode:setActiveNode(node)
	--[[
	���û�ڵ㣻�÷������򸸽ڵ����û�ڵ�
	@param	node��
	@type	node��object of BevNode or nil
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
	�ڵ�ת��ʱ�Ĵ���
	@param	input��
	@type	input��any
	@param	return��
	@type	return��BevRunningStatus
	--]]
	return self:onTransition_(input)
end

function BevNode:getName()
	--[[
	��ȡ���֣�ͨ�����ڵ�����Ϊ����
	@param	return��
	@type	return��string
	--]]
	return self._objName
end

function BevNode:setPrecondition(precondition)
	--[[
	������Ϊ�����ⲿǰ������;�÷������ͷžɵ��ⲿǰ������
	@param	precondition��
	@type	precondition��object of BevPrecondition or nil
	--]]
	if self._precondition ~= precondition then
		release(self._precondition)
		self._precondition = precondition
	end
end

function BevNode:checkIndex_(index)
	--[[
	����±������Ƿ�Ϸ�
	@param	index��
	@type	index��int
	@param	return��
	@type	return��boolean
	--]]
	return index >= 1 and index <= #self.childNodes_
end

function BevNode:onLoad_(...)
	--[[
	�������ûص�������������ʵ��
	@param	arg:
	@type	arg: Args
	--]]
end

function BevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����������ʵ��
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
	--]]
	return true
end

function BevNode:onTransition_(input)
	--[[
	�ڵ�ת��ʱ�Ļص�����������ʵ��
	@param	input��
	@type	input��any
	--]]
end

function BevNode:onTick_(input, output)
	--[[
	��Ϊ���Ĵ���ص�������������ʵ��
	@param	input��
	@type	input��any
	@param	output��
	@type	output��object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	return BevRunningStatus.Finish
end
