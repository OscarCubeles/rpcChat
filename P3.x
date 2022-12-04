program RPC{
	version FIRST_VERSION{
		void write_chat(string) = 0;
		string get_chat(int) = 1;
	} = 1;
} = 0x20000001;
