--[[Writen by Luada

提供一个高效的根据id或者name来索引对象的管理类

--]]

require "base.class"


IIdNameObj = interface(nil,
			"getId", 				---getId(), 获取对象的Id值（集合体内值唯一）
			"getName"				---getName()，获取对象的名字（集合体内值唯一）
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
	根据id来查找对象
	@param	id：id值，集合体内值唯一
	@type	id：int
	@param	return：
	@type	return：object of IIdNameObj or nil
	--]]
	return self._idMapObjs[id]
end

function IdNameMap:getObjByName(name)
	--[[
	根据名字来查找对象
	@param	name: 集合体内值唯一
	@type	name：string
	@param	return：
	@type	return：object of IIdNameObj or nil
	--]]
	return self._nameMapObjs[name]
end

function IdNameMap:add(idNameObj)
	--[[
	在集合体内添加新对象
	@param	idNameObj:
	@type	idNameObj: object of IIdNameObj
	--]]
	self._idMapObjs[idNameObj:getId()] = idNameObj
	self._nameMapObjs[idNameObj:getName()] = idNameObj
end

function IdNameMap:clear(releaseFn)
	--[[
	清空内部数据
	@param	releaseFn：元素清空函数
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
	在集合体内删除对象
	@param	idNameObj:
	@type	idNameObj: object of IIdNameObj
	@param	releaseFn：元素清空函数
	@type	releaseFn: Functor or nil
	--]]
	self._idMapObjs[idNameObj:getId()] = nil
	self._nameMapObjs[idNameObj:getName()] = nil
	
	if releaseFn ~= nil then
		releaseFn(idNameObj)
	end
end