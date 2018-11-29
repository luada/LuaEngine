--[[Writen by Luada

�������л��ӿ�,���ṩserialize��unserialize�������ڶ�Table����������л��ͷ����л�����

Exported API:
	serialize(t)
	unserialize(str)
--]]

require "base.interface"

ISerializable = interface(nil,
	"writeObject",
	"readObject")

--------------------------------------------------------------------------------
local function setArrayValue(array, key, value)
	--[[
	����userdata����ֵ
	@param	array��
	@type	array: userdata
	@param	key: ��ֵ��Ϊintʱ����1��ʼ
	@type	key: string or int
	@param	value: ������ֵ
	@type	value: any
	--]]
	if type(key) == "number" then
		local idx = tonumber(key) - 1
		array[idx] = value
	else
		if type(array[key]) ~= "nil" then
			array[key] = value
		end
	end
end

--------------------------------------------------------------------------------
local function getArrayValue(array, key)
	--[[
	��ȡuserdata�����idx��Ԫ��ֵ
	@param	array��
	@type	array: userdata
	@param	key: ��ֵ��Ϊintʱ����1��ʼ
	@type	key: string or int
	@param	return: userdata�����idx��Ԫ��ֵ
	@type	return: any
	--]]
	if type(key) == "number" then
		local idx = tonumber(key) - 1
		return array[idx]
	else
		return array[key]
	end
end

--------------------------------------------------------------------------------
local function copyData(tbl, output)
	--[[
	��ȿ���tblֵ��output
	@param	tbl:
	@type	tbl: table
	@param	output:
	@type	output: userdata
	--]]
	for k, v in pairs(tbl) do
		if type(v) == "table" then
			local status, ret = pcall(getArrayValue, output, k)
			if not status then
				break
			end
			copyData(v, ret)
		else
			local status, ret = pcall(setArrayValue, output, k, v)
			if not status then
				break
			end
		end
	end
end

--------------------------------------------------------------------------------
function serialize(input)
	--[[
	���л�
	@param	input: �����л�������Դ
	@type	input: any
	@param	return�����л��Ľ��
	@type	return��string
	--]]
	local mark={}
	local assign={}
	
	local function table2str(tbl, parent, ud, mt, ret, frmArray, tabs)
		mark[tbl] = parent
		local ret = ret or {}
		local preTabs = tabs
		tabs = tabs .. "\t"
		
		---print("begin table2str")
		
		if mt ~= nil then
			local mtGetters = mt[".get"]
			if type(mtGetters) == "table" then
				---print("B mt----->")
				table2str(mtGetters, parent, ud, getmetatable(mt), ret, false, preTabs)
				---print("E mt----->")
			end
		end
		
		if frmArray then
			local i = 0
			---print("B frmArray")
			while true do
				i = i + 1
				local s, v = pcall(getArrayValue, tbl, i)
				if not s then
					break
				end
				
				local k = tostring(i)
				local dotkey = parent.."["..k.."]"
				local t = type(v)
				if t == "userdata" then
					---print("B UD frmArray--->")
					local mt = getmetatable(v)
					local getters = mt and mt[".get"] or nil
					if type(getters) == "table" then
						table.insert(ret, table2str(getters, dotkey, v, getmetatable(mt), nil, false, tabs))
					end
					---print("E UD frmArray--->")
				elseif t == "function" or t == "thread" or t == "proto" or t == "upval" then
					---ignore
				elseif t == "table" then
					table.insert(ret, table2str(v, dotkey, nil, nil, nil, false, tabs))
				elseif t == "string" then
					table.insert(ret, string.format("%s%q", tabs, v))
				else
					table.insert(ret, string.format("%s%s", tabs, tostring(v)))
				end
			end
		elseif table.isArray(tbl) then
			table.foreach(tbl, function(i, v)
				local k = tostring(i)
				local dotkey = parent.."["..k.."]"
				local t = type(v)
				---print("isArray tbl--->", k, t)
				if t == "userdata" or t == "function" or t == "thread" or t == "proto" or t == "upval" then
					---ignore
				elseif t == "table" then
					if mark[v] then
						table.insert(assign, dotkey.."="..mark[v])
					else
						table.insert(ret, table2str(v, dotkey, nil, nil, nil, false, tabs))
					end
				elseif t == "string" then
					table.insert(ret, string.format("%s%q", tabs, v))
				else
					table.insert(ret, string.format("%s%s", tabs, tostring(v)))
				end
			end)
		else
			table.foreach(tbl, function(f, v)
				local k = type(f)=="number" and "["..f.."]" or f
				local dotkey = parent..(type(f)=="number" and k or "."..k)
				local t = type(v)
				---print("---->", k, t)
				if t == "function" then
					if ud ~= nil then
						local getV = v(ud)
						local tV = type(getV)
						if tV == "userdata" then
							---print("B UD--->")
							local mt = getmetatable(getV)
							local getters = mt and mt[".get"] or nil
							if type(getters) == "table" then
								table.insert(ret, string.format("%s%s = %s", tabs, k, table2str(getters, dotkey, getV, getmetatable(mt), nil, false, tabs)))
							end
							---print("E UD--->")
						elseif tV == "string" then
							table.insert(ret, string.format("%s%s = %q", tabs, k, tostring(getV)))
						else
							table.insert(ret, string.format("%s%s = %s", tabs, k, tostring(getV)))
						end
					else
						---ignore
					end
				elseif t == "thread" or t == "userdata" or t == "proto" or t == "upval" then
					---ignore
				elseif t == "table" then
					if ud ~= nil then
						v = ud[tostring(f)]
					end
					if mark[v] then
						table.insert(assign, dotkey.."="..mark[v])
					else
						table.insert(ret, string.format("%s%s = %s", tabs, k, table2str(v, dotkey, nil, nil, nil, ud ~= nil, tabs)))
					end
				elseif t == "string" then
					table.insert(ret, string.format("%s%s = %q", tabs, k, v))
				else
					table.insert(ret, string.format("%s%s = %s", tabs, k, tostring(v)))
				end
			end)
		end
		
		---print("End table2str")
		return string.format("%s{%s%s}", preTabs == "\n" and "" or preTabs, table.concat(ret,","), preTabs)
	end

	local t = type(input)
	local tabs = "\n"
	if t == "table" then
		return string.format("%s%s",  table2str(input, "_" , nil, nil, nil, false, tabs), table.concat(assign," "))
	elseif t == "userdata" then
		local mt = getmetatable(input)
		local getters = mt[".get"]
		if type(getters) == "table" then
			return string.format("%s%s",  table2str(getters, "_", input, getmetatable(mt), nil, false, tabs), table.concat(assign," "))
		end
	end
	---default
	return tostring(input)
end

--------------------------------------------------------------------------------
function unserialize(str, output)
	--[[
	�����л�
	@param	str: �����л��ַ���
	@type	str��string
	@param	output: ������л����Ϊtable or userdata���򿽱�����
	@type	output��table or userdata or nil
	@param	return�������л��Ľ��
	@type	return��any
	--]]
	local strCode = string.format("do local _=%s return _ end", str)
	local status, ret = pcall(loadstring(strCode))
	if status then
		if type(ret) == "table" then
			local t = type(output)
			if t == "table" then
				table.deepCopy(ret, output, true)
			elseif t == "userdata" then
				copyData(ret, output)
			end
		end
		return ret
	end
	return nil
end
