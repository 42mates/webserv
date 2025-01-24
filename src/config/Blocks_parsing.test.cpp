/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testsBlocks_parsing.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:39:05 by sokaraku          #+#    #+#             */
/*   Updated: 2025/01/24 00:05:02 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"
#include "Blocks.hpp"
#include "WebServ.hpp"
#include "Config.hpp"
#include <cstdarg>

void testParseListen();
void testParseServerName();
void testParseErrorPage();
void testParseClientMaxBodySize();
void testParseRoot();
void testParseMethods();
void testParseDirectoryListing();
void testParseIndexFile();
void testParseCgiPath();
void testParseUploadDir();
void testParseHttpRedirect();
void testParseReturn();
void runAll();

map<string, void(*)()> functionMap;

void initializeFunctionMap() {
	functionMap["LISTEN"] = testParseListen;
    functionMap["SERVERNAME"] = testParseServerName;
    functionMap["ERRORPAGE"] = testParseErrorPage;
    functionMap["CLIENTMAXBODYSIZE"] = testParseClientMaxBodySize;
    functionMap["ROOT"] = testParseRoot;
    functionMap["METHODS"] = testParseMethods;
    functionMap["DIRECTORYLISTING"] = testParseDirectoryListing;
    functionMap["INDEXFILE"] = testParseIndexFile;
    functionMap["CGIPATH"] = testParseCgiPath;
    functionMap["UPLOADDIR"] = testParseUploadDir;
    functionMap["HTTPREDIRECT"] = testParseHttpRedirect;
    functionMap["RETURN"] = testParseReturn;
}

vector<string> createStringVector(int count, ...);
vector<string> empty = createStringVector(1, "");
struct ServerConfig	svr;
struct RouteConfig route;
string	path("[PATH]");
string context("[CONTEXT]");
ServerBlock serverBlock(&svr, path);
LocationBlock locationBlock(route, path);


void	testBlocks_parsing( void )
{
	vector<string>	val;
	string			buf;

	initializeFunctionMap();
	while (1)
	{
		buf.clear();
		cout << "Enter the block's name (case insensitive)\n";
		if ((!getline(cin, buf)))
			break ;
		transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
		if (buf == "EXIT")
			break ;
		else if (buf == "ALL")
		{
			runAll();
			continue ;
		}
		if (functionMap.count(buf))
		{
			cout << "\033[2J\033[H";
			functionMap[buf]();
		}
		else
			cout << "Wrong name (ctrl+D or \"EXIT\" to quit)\n";
	}
}

void	printVector(vector<string>& v)
{
	for (size_t i = 0; i < v.size(); i++)
		cout << GREY << "v[" << i << "] = " << NC << v[i] << "\n";
}

vector<string> createStringVector(int count, ...)
{
    vector<string> vec;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        vec.push_back(va_arg(args, const char*));
    }
    va_end(args);
    return vec;
}

void testParseListen()
{
    vector<string> port1 = createStringVector(1, "8080");
    vector<string> out_of_bound = createStringVector(1, "99999");
    vector<string> alpha_char = createStringVector(1, "80a80");
	vector<string> negative_value = createStringVector(1, "-1");
	vector<string> too_many_value = createStringVector(2, "1", "80");

    cout << GREY << "test1" << NC << "\n";
    printVector(port1);
    try {
        serverBlock.parseListen(port1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(out_of_bound);
    try {
        serverBlock.parseListen(out_of_bound);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(alpha_char);
    try {
        serverBlock.parseListen(alpha_char);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
   cout << GREY << "test4" << NC << "\n";
    printVector(negative_value);
    try {
        serverBlock.parseListen(negative_value);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
   cout << GREY << "test5" << NC << "\n";
    printVector(too_many_value);
    try {
        serverBlock.parseListen(too_many_value);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
   cout << GREY << "test6" << NC << "\n";
    printVector(empty);
    try {
        serverBlock.parseListen(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseServerName()
{
    vector<string> test_server_name1 = createStringVector(1, "example.com");
    vector<string> test_server_name2 = createStringVector(1, "localhost");
    vector<string> test_server_name3 = createStringVector(3, "localhost", "anotherHost", "MarinIloveYou");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_server_name1);
    try {
        serverBlock.parseServerName(test_server_name1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_server_name2);
    try {
        serverBlock.parseServerName(test_server_name2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(test_server_name3);
    try {
        serverBlock.parseServerName(test_server_name3);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
   cout << GREY << "test4" << NC << "\n";
    printVector(empty);
    try {
        serverBlock.parseServerName(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseErrorPage()
{
    vector<string> test_error_page = createStringVector(5, "404", "403", "407", "310", "/404.html");
    vector<string> out_of_bound = createStringVector(2, "700", "/500.html");
    vector<string> negative_return = createStringVector(2, "-500", "/500.html");
	vector<string> alpha_char = createStringVector(2, "404a", "/500.html");
	vector<string> wrong_num_args = createStringVector(1, "404");
	vector<string> file_not_at_end = createStringVector(3, "404", "/404", "405");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_error_page);
    try {
        serverBlock.parseErrorPage(test_error_page);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(out_of_bound);
    try {
        serverBlock.parseErrorPage(out_of_bound);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(negative_return);
    try {
        serverBlock.parseErrorPage(negative_return);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(alpha_char);
    try {
        serverBlock.parseErrorPage(alpha_char);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test5" << NC << "\n";
    printVector(wrong_num_args);
    try {
        serverBlock.parseErrorPage(wrong_num_args);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test6" << NC << "\n";
    printVector(file_not_at_end);
    try {
        serverBlock.parseErrorPage(file_not_at_end);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test7" << NC << "\n";
    printVector(empty);
    try {
        serverBlock.parseErrorPage(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseClientMaxBodySize()
{
    vector<string> test_client_max_body_size1 = createStringVector(1, "10M");
    vector<string> test_client_max_body_size2 = createStringVector(1, "10m");
    vector<string> test_client_max_body_size3 = createStringVector(1, "1024");
    vector<string> not_valid_unit = createStringVector(1, "1024mk");
    vector<string> not_valid_size = createStringVector(1, "-1024m");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_client_max_body_size1);
    try {
        serverBlock.parseClientMaxBodySize(test_client_max_body_size1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_client_max_body_size2);
    try {
        serverBlock.parseClientMaxBodySize(test_client_max_body_size2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(test_client_max_body_size3);
    try {
        serverBlock.parseClientMaxBodySize(test_client_max_body_size3);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(not_valid_unit);
    try {
        serverBlock.parseClientMaxBodySize(not_valid_unit);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test5" << NC << "\n";
    printVector(not_valid_size);
    try {
        serverBlock.parseClientMaxBodySize(not_valid_size);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test6" << NC << "\n";
    printVector(empty);
    try {
        serverBlock.parseClientMaxBodySize(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseRoot()
{
    struct RouteConfig svr;

    vector<string> test_root1 = createStringVector(1, "/var/www/html");
    vector<string> test_root2 = createStringVector(1, "/usr/local/www");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_root1);
    try {
        locationBlock.parseRoot(test_root1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_root2);
    try {
        locationBlock.parseRoot(test_root2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseRoot(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseMethods()
{
    struct RouteConfig svr;

    vector<string> test_methods1 = createStringVector(2, "GET", "post");
    vector<string> test_methods2 = createStringVector(1, "delete");
	vector<string> unknown_method = createStringVector(4, "geT", "POST", "HEAD", "delete");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_methods1);
    try {
        locationBlock.parseMethods(test_methods1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_methods2);
    try {
        locationBlock.parseMethods(test_methods2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseMethods(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(unknown_method);
    try {
        locationBlock.parseMethods(unknown_method);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseDirectoryListing()
{
    struct RouteConfig svr;

    vector<string> test_directory_listing1 = createStringVector(1, "on");
    vector<string> test_directory_listing2 = createStringVector(1, "off");
	vector<string> not_valid_name = createStringVector(1, "enable");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_directory_listing1);
    try {
        locationBlock.parseDirectoryListing(test_directory_listing1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_directory_listing2);
    try {
        locationBlock.parseDirectoryListing(test_directory_listing2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(not_valid_name);
    try {
        locationBlock.parseDirectoryListing(not_valid_name);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseDirectoryListing(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseIndexFile()
{
    struct RouteConfig svr;

    vector<string> test_index_file1 = createStringVector(1, "index.html");
    vector<string> test_index_file2 = createStringVector(1, "home.html");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_index_file1);
    try {
        locationBlock.parseIndexFile(test_index_file1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_index_file2);
    try {
        locationBlock.parseIndexFile(test_index_file2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseIndexFile(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseCgiPath()
{
    struct RouteConfig svr;

    vector<string> testCgiPath1 = createStringVector(1, "/usr/bin/python3");
    vector<string> testCgiPath2 = createStringVector(1, "/usr/local/bin/php");

    cout << GREY << "test1" << NC << "\n";
    printVector(testCgiPath1);
    try {
        locationBlock.parseCgiPath(testCgiPath1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(testCgiPath2);
    try {
        locationBlock.parseCgiPath(testCgiPath2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseCgiPath(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseUploadDir()
{
    struct RouteConfig svr;

    vector<string> test_upload_dir1 = createStringVector(1, "/var/www/uploads");
    vector<string> test_upload_dir2 = createStringVector(1, "/usr/local/uploads");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_upload_dir1);
    try {
        locationBlock.parseUploadDir(test_upload_dir1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_upload_dir2);
    try {
        locationBlock.parseUploadDir(test_upload_dir2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseUploadDir(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseHttpRedirect()
{
    struct RouteConfig svr;

    vector<string> test_http_redirect1 = createStringVector(2, "301", "https://example.com");
    vector<string> test_http_redirect2 = createStringVector(2, "302", "https://example.org");
	vector<string> too_many_args = createStringVector(3, "301", "302", "https://example.com");
	vector<string>	out_of_bound_return_value = createStringVector(2, "299", "https://example.com");
	vector<string>	alpha_char_in_return = createStringVector(2, "30a1", "https://example.com");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_http_redirect1);
    try {
        locationBlock.parseHttpRedirect(test_http_redirect1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_http_redirect2);
    try {
        locationBlock.parseHttpRedirect(test_http_redirect2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(too_many_args);
    try {
        locationBlock.parseHttpRedirect(too_many_args);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(out_of_bound_return_value);
    try {
        locationBlock.parseHttpRedirect(out_of_bound_return_value);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test5" << NC << "\n";
    printVector(alpha_char_in_return);
    try {
        locationBlock.parseHttpRedirect(alpha_char_in_return);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test6" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseHttpRedirect(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void testParseReturn()
{
    struct RouteConfig svr;

    vector<string> test_return1 = createStringVector(2, "200", "/index.html");
    vector<string> test_return2 = createStringVector(2, "404", "/404.html");
	vector<string> too_many_args = createStringVector(3, "301", "302", "https://example.com");
	vector<string> out_of_bound_return_value = createStringVector(2, "-1","https://example.com");
	vector<string>	alpha_char_in_return = createStringVector(2, "30a1", "https://example.com");

    cout << GREY << "test1" << NC << "\n";
    printVector(test_return1);
    try {
        locationBlock.parseReturn(test_return1);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test2" << NC << "\n";
    printVector(test_return2);
    try {
        locationBlock.parseReturn(test_return2);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test3" << NC << "\n";
    printVector(too_many_args);
    try {
        locationBlock.parseReturn(too_many_args);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test4" << NC << "\n";
    printVector(out_of_bound_return_value);
    try {
        locationBlock.parseReturn(out_of_bound_return_value);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test5" << NC << "\n";
    printVector(alpha_char_in_return);
    try {
        locationBlock.parseReturn(alpha_char_in_return);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
    cout << GREY << "test6" << NC << "\n";
    printVector(empty);
    try {
        locationBlock.parseReturn(empty);
    } catch (const exception &e) {
       cerr << "webserv: " << e.what() << '\n';
    }
}

void runAll()
{
	cout << MAGENTA << "\t\t\tLISTEN DIRECTIVE\n" << NC;
	testParseListen();
	cout << MAGENTA << "\t\t\tSERVER_NAME DIRECTIVE\n" << NC;
	testParseServerName();
	cout << MAGENTA << "\t\t\tERROR_PAGE DIRECTIVE\n" << NC;
	testParseErrorPage();
	cout << MAGENTA << "\t\t\tCLIENT_MAX_BODY_SIZE DIRECTIVE\n" << NC;
	testParseClientMaxBodySize();
	cout << MAGENTA << "\t\t\tROOT DIRECTIVE\n" << NC;
	testParseRoot();
	cout << MAGENTA << "\t\t\tMETHODS DIRECTIVE\n" << NC;
	testParseMethods();
	cout << MAGENTA << "\t\t\tDIRECTORY_LISTING DIRECTIVE\n" << NC;
	testParseDirectoryListing();
	cout << MAGENTA << "\t\t\tINDEX_FILE DIRECTIVE\n" << NC;
	testParseIndexFile();
	cout << MAGENTA << "\t\t\tCGI_PATH DIRECTIVE\n" << NC;
	testParseCgiPath();
	cout << MAGENTA << "\t\t\tUPLOAD_DIR DIRECTIVE\n" << NC;
	testParseUploadDir();
	cout << MAGENTA << "\t\t\tHTTP_REDIRECT DIRECTIVE\n" << NC;
	testParseHttpRedirect();
	cout << MAGENTA << "\t\t\tRETURN DIRECTIVE\n" << NC;
	testParseReturn();
}