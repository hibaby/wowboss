
local stock = require "me.stock"
require "me.math"

local base = _ENV
local _mode = {}
local _ENV = _mode

function OnCallFunc()
	stock.UpdateStk()	
end

return _mode
