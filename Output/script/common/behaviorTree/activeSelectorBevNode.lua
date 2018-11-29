--[[Created by Luada
����ѡ��ڵ�

���Խ׶Σ�
	ǰ����������ͨ��ʱ������ֵ��Ϊtrue. �ӵ�һ���ӽڵ㿪ʼ���α������е��ӽڵ㣬������test�����������ִ��ڿ���
	���е�Ҷ�ӽڵ㣨��Ϊ�ڵ㣩�����ü���ڵ㣨��Ҷ�ӽڵ�����ɣ���ֹͣ������
	
����׶Σ�
	���ü���ڵ��tick����
--]]

ActiveSelectorBevNode = class(ComponentBevNode)

function ActiveSelectorBevNode:__init(name)
	ComponentBevNode.__init(self, name)
end

function ActiveSelectorBevNode:__release()
end

function ActiveSelectorBevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return������˺���ʱ��ǰ�������Ѿ�ͨ��
	@type	return��boolean(true)
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
	��Ϊ���Ĵ���ص�����
	@param	input��
	@type	input��any
	@param	output��
	@type	output��object or table
	@param	return:
	@type	return: BevRunningStatus
	--]]
	local excStatus = BevRunningStatus.Finish
	if self.activeNode_ ~= nil then
		excStatus = self.activeNode_:tick(input, output)
	end
	return excStatus
end
