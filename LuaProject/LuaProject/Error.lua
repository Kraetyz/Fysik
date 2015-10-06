--Errorhandling
function ErrorHandler(errmsg)
errmsg = errmsg or ""
	return errmsg .. "\n" .. debug.traceback()
end