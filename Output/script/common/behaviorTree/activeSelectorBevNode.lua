--[[Created by Luada
激活选择节点

测试阶段：
	前提条件测试通过时，返回值设为true. 从第一个子节点开始依次遍历所有的子节点，调用其test方法，当发现存在可以
	运行的叶子节点（行为节点），设置激活节点（由叶子节点来完成），停止遍历，
	
处理阶段：
	调用激活节点的tick方法
--]]

ActiveSelectorBevNode = class(ComponentBevNode)

function ActiveSelectorBevNode:__init(name)
	ComponentBevNode.__init(self, name)
end

function ActiveSelectorBevNode:__release()
end

function ActiveSelectorBevNode:onTest_(input)
	--[[
	测试函数的回调处理
	@param	input：
	@type	input：any
	@param	return：处理此函数时，前提条件已经通过
	@type	return：boolean(true)
	--]]
	for _, v in ipairs(self.childNodes_) do
		---local nowTime = YXS_PVE.GetElapse()
		if v:test(input) then
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
			return true
		else
			---table.insert(AICostTime, {v:getName(), (YXS_PVE.GetElapse() - nowTime) * 1000})
		end
	end
	return false
end

function ActiveSelectorBevNode:onTick_(input, output)
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
	if self.activeNode_ ~= nil then
		excStatus = self.activeNode_:tick(input, output)
	end
	return excStatus
end
