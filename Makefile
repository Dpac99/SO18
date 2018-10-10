.SILENT: all clean

all:
		make -C ./shell
		echo "WARNING: CircuitRouter-SimpleShell must be run from inside shell/ directory"

clean:
		make clean -C ./shell