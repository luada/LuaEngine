--[[Writen by Luada

�ṩ��StringBuffer��,�����Ż�С�ַ�����ƴ�Ӳ���

Exported API:
	StringBuffer(string)
	toString(value)

Example:
	local s = StringBuffer("<<object>>=")
	s = s .. "{" .. "name"
	s = s + "= 'test'"
	print(s)
--]]

require "base.base"

--------------------------------------------------------------------------------
local function sbAppend(sb, s)
	--[[
	��ֵs׷�ӵ�StringBuffer sb��
	@param	sb��
	@type	sb��StringBuffer
	@param	s:
	@type	s: any
	@param	return: ׷��ֵs���sb
	@type	return��StringBuffer
	--]]
	table.insert(sb, tostring(s))
	for i = #sb-1, 1, -1 do
		if #(sb[i]) > #(sb[i+1]) then
			break
		end
		sb[i] = sb[i] .. table.remove(sb)
	end
	return sb
end
--------------------------------------------------------------------------------
local classMT = {
	__add = sbAppend,
	__concat = sbAppend,
	__tostring = table.concat
}
--------------------------------------------------------------------------------
local function sbNew(_, s)
	local sb = define(classMT, {
		append = function(self, s)
			self = self .. s
			return self
		end
	})
	table.insert(sb, s and tostring(s) or "")
	return sb
end
--------------------------------------------------------------------------------
StringBuffer = define { __call = sbNew }
--------------------------------------------------------------------------------
function toString(value, default)
	--[[
	��value�ַ�����; valueΪnilʱ,����Ĭ��ֵdefault
	@param	value:
	@type	value: any
	@param	default:valueΪnilʱ��Ĭ��ֵ
	@type	default: any
	@param	return:
	@tpe	return: string
	--]]
	local str = StringBuffer()
	if type(value) ~= "table" then
		if type(value) == "string" then
			str = str .. string.format("%q", value)
		elseif value == nil then
			str = str .. tostring(default)
		else
			str = str .. tostring(value)
		end
	else
		str = str .. '{'
		local separator = ""
		if table.isArray(value) then
			table.foreach(value, function (i, v)
				str = str .. separator .. string.format("%s", toString(v))
				separator = ", "
			end)
		else
			table.foreach(value, function (f, v)
				if type(f) == "number" then
					str = str .. separator .. string.format("[%d] = %s", f, toString(v))
				else
					str = str .. separator .. string.format("%s = %s", tostring(f), toString(v))
				end
				separator = ", "
			end)
		end
		str = str .. '}'
	end
	return tostring(str)
end