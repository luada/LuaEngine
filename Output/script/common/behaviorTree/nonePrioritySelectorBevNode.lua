--[[Created by Luada
�������ȼ���ѡ��ڵ�

���Խ׶Σ�
	�ȵ�����һ�����е��ӽڵ㣨�����ڣ���test����������������У�������˱���ýڵ�
	������������true������������У�������ѡ��ͬ�����ȼ���ѡ��ڵ��ѡ��ʽ��

����׶Σ�
	���ÿ������е��ӽڵ��Tick���������������ص�����״̬��Ϊ���������״̬����
--]]
NonePrioritySelectorBevNode = class(PrioritySelectorBevNode)

function NonePrioritySelectorBevNode:__init(name)
	PrioritySelectorBevNode.__init(self, name)
end

function NonePrioritySelectorBevNode:__release()
end

function NonePrioritySelectorBevNode:onTest_(input)
	--[[
	���Ժ����Ļص�����
	@param	input��
	@type	input��any
	@param	return��
	@type	return��boolean
	--]]
	if self:checkIndex_(self.curIdx_) then
		if self.childNodes_[self.curIdx_]:test(input) then
			return true
		end
	end
	return PrioritySelectorBevNode.onTest_(self, input)
end
