// ntparser_cgi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//2:09 AM 11/18/2018 START
//4:11 AM 11/18/2018 END
#include "util.h"
using namespace std;
using namespace NTemplate::Compiler;
int main(int argc, char *argv[], char*envp[]) {
	char* CONTENT_TYPE = get_content_type();
	std::cout << "Content-Type: " << CONTENT_TYPE << "\n\n" << "\r\n";
	req_method method = determine_req_method();
	if (method == req_method::UNSUPPORTED) {
		printf("Status-Code: 405-Method Not Allowed\n\n");
		std::cout << "This method " << get_env("REQUEST_METHOD") << "not supported!!!" << "\r\n";
		return 0;
	}
	auto query_string = read_query_string();
	if (method == req_method::POST) {
		int ilen = atoi(get_env("CONTENT_LENGTH"));
		char* bufp = new char[ilen + 1];
		size_t size = sizeof bufp;
		malloc(size);
		fread(bufp, size, 1, stdin);
		printf("The POST data is<P>%s\n", bufp);
		free(bufp);
		return 0;
	}
	char* PATH_INFO = get_env("PATH_INFO");
	char* HTTP_COOKIE = get_env("HTTP_COOKIE");
	char* REMOTE_ADDR = get_env("REMOTE_ADDR");
	char* PATH_TRANSLATED = get_env("PATH_TRANSLATED");
	char* execute_path = argv[0];
	std::string ex_dir;
	std::string ex_name;
	split_file_name(execute_path, ex_dir, ex_name);
	ex_dir = ex_dir + "\\";
	std::string root_dir;
	server_physical_path(PATH_TRANSLATED, PATH_INFO, root_dir);
	template_result* tr = new template_result();
	parser_settings* ps = new parser_settings();
	root_dir = root_dir + "\\";
	ps->dir = root_dir.c_str();
	ps->page_path = PATH_INFO;
	tr->remove_new_line = false;
	ntemplate_parse_x(ps, tr);
	if (tr->is_error == true) {
		printf(tr->err_msg.c_str());
		delete tr; delete ps;
		print_envp(envp);
		return 0;
	}
	else if (tr->is_script_template == true) {
		run_template_x(tr, "{MY__DATA:1}", ex_dir.c_str());
	}
	printf(tr->t_source.c_str());
	delete tr; delete ps;
	return 0;
};