#include "Request.hpp"

Response Request::handleDELETE()
{
	Response response;

	try
	{
		// Get the file path from the URI
		string file_path = getFilePath(_route_conf.root + _uri);

		// Check if the file exists and is accessible
		struct stat buffer;
		if (stat(file_path.c_str(), &buffer) != 0)
		{
			cout << "DELETE request rejected: File \"" << file_path << "\" does not exist or is inaccessible." << endl;
			if (errno == EACCES)
				throw ResponseException(Response("403"), "\"" + file_path + "\": " + strerror(errno));
			else
				throw ResponseException(Response("404"), "\"" + file_path + "\": " + strerror(errno));
		}

		// Attempt to delete the file
		if (remove(file_path.c_str()) != 0)
		{
			cout << "DELETE request rejected: Failed to delete file \"" << file_path << "\" due to insufficient permissions." << endl;
			throw ResponseException(Response("403"), "Failed to delete the file (permission denied)");
		}

		// Set response status to 204 (No Content) as the file was successfully deleted
		response.setCode("200");
		response.setBody("File deleted successfully: " + _uri);
		cout << "DELETE request accepted: File \"" << file_path << "\" was successfully deleted." << endl;
	}
	catch (const ResponseException &e)
	{
		cerr <<  "response: handleDELETE(): " << e.what() << endl;
		response = e.getResponse();
	}
	catch (const exception &e)
	{
		cerr <<  "handleDELETE(): Unexpected error: " << e.what() << endl;
		response.setCode("500");
		response.setBody("Internal Server Error");
	}

	return response;
}
