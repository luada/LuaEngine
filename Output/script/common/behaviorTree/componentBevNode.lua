--[[Created by Luada
����ڵ�Ļ���
--]]

ComponentBevNode = class(BevNode)

function ComponentBevNode:__init(name)
	--[[
	��ʼ��
	@param	name: �����������
	@type	name: string
	--]]
	BevNode.__init(self, name, nil)
end

function ComponentBevNode:__release()
end
