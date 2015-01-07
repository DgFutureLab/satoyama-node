libpath="$HOME/Documents/Arduino/libraries/SatoyamaChibiLib"
if [ -d "$libpath" ]; then
	cp -r . "$libpath/"	
else
	mkdir "$libpath"
	cp -r . "$libpath/"
fi

