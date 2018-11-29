--[[Writen by Luada

�ṩһ����Ч�ĸ���id����name����������Ĺ�����

--]]

require "base.class"


IIdNameObj = interface(nil,
			"getId", 				---getId(), ��ȡ�����Idֵ����������ֵΨһ��
			"getName"				---getName()����ȡ��������֣���������ֵΨһ��
			)

--------------------------------------------------------------------------------
IdNameMap = class()

function IdNameMap:__init()
	self._idMapObjs = {}
	self._nameMapObjs = {}
end

function IdNameMap:__release()
	self._idMapObjs = nil
	self._nameMapObjs = nil
end

function IdNameMap:getObjById(id)
	--[[
	����id�����Ҷ���
	@param	id��idֵ����������ֵΨһ
	@type	id��int
	@param	return��
	@type	return��object of IIdNameObj or nil
	--]]
	return self._idMapObjs[id]
end

function IdNameMap:getObjByName(name)
	--[[
	�������������Ҷ���
	@param	name: ��������ֵΨһ
	@type	name��string
	@param	return��
	@type	return��object of IIdNameObj or nil
	--]]
	return self._nameMapObjs[name]
end

function IdNameMap:add(idNameObj)
	--[[
	�ڼ�����������¶���
	@param	idNameObj:
	@type	idNameObj: object of IIdNameObj
	--]]
	self._idMapObjs[idNameObj:getId()] = idNameObj
	self._nameMapObjs[idNameObj:getName()] = idNameObj
end

function IdNameMap:clear(releaseFn)
	--[[
	����ڲ�����
	@param	releaseFn��Ԫ����պ���
	@type	releaseFn: Functor or nil
	--]]
	self._nameMapObjs = {}
	if releaseFn ~= nil then
		for _, obj in pairs(self._idMapObjs) do
			releaseFn(obj)
		end
	end
	self._idMapObjs = {}
end

function IdNameMap:remove(idNameObj, releaseFn)
	--[[
	�ڼ�������ɾ������
	@param	idNameObj:
	@type	idNameObj: object of IIdNameObj
	@param	releaseFn��Ԫ����պ���
	@type	releaseFn: Functor or nil
	--]]
	self._idMapObjs[idNameObj:getId()] = nil
	self._nameMapObjs[idNameObj:getName()] = nil
	
	if releaseFn ~= nil then
		releaseFn(idNameObj)
	end
end