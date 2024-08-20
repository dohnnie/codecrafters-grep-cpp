#include <iostream>
#include <string>

bool contains_char(const char c, const char flag) {
    //for numbers
    if(flag == 'd') {
        if( (c >= '0') && (c <= '9'))
            return true;
    }
    //for alphanumeric characters
    else if(flag == 'w') {
        if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ' '))
	        return true; 
    }

    return false;
}

bool match_char(const char* text, const char *regexp) {
    if(*regexp == '\0') return 1;

    if(text[0] == '\0') return 0;
    
    if(regexp[0] == '\\' && regexp[1] != '\0') {
        if(contains_char(text[0], regexp[1])) return match_char(text + 1, regexp + 2);
    }

    if(*regexp == *text && regexp[1] == '+') {
        if(text[0] != text[1])
            return match_char(text + 1,regexp + 2);

        return match_char(text + 1, regexp);
    }
    
    if(text[0] == regexp[0])
        return match_char(text + 1, regexp + 1);

    return match_char(text + 1, &regexp[0]);
}

bool match(const char* text, const char *regexp) {

    if(regexp[0] == '\0')
        return false;
    else if(regexp[0] == '\\') {
        return match_char(text, regexp);
    }
    else if(contains_char(regexp[0], 'w')) {
        return match_char(text, regexp);
    }
    return false;
}

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if (pattern.length() == 1) {
        return input_line.find(pattern) != std::string::npos;
    }
    else if(pattern[pattern.length() - 1] == '$') {
        for(int i = input_line.length() - 1; i >= 0; --i) {
            if(input_line[i] != pattern[i])
                return false;
        }

        return true;
    }
    else if(pattern[0] == '[') {
    	int index = 1;
        char c = pattern[index];
	    std::string plus_char = "";

        if(pattern[1] == '^')
            index++;

	    while(c != ']') {
            plus_char += c;
            c = pattern[index++];
	    }

        if(pattern[1] == '^')
            return !(input_line.find_first_of(plus_char) != std::string::npos);

        return input_line.find_first_of(plus_char) != std::string::npos;
    }
    else if(pattern[0] == '^') {
        for(int i = 0; i < pattern.length(); i++) {
            if(input_line[i] != pattern[i + 1])
                return false;
        }

        return true;
    }
    else if(pattern[0] == '\\' || contains_char(pattern[0], 'w')) {
        return match(&input_line[0], &pattern[0]);
    }
    else {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
   
    std::string input_line;
    std::getline(std::cin, input_line);
    
    try {
    	if (match_pattern(input_line, pattern)) {
        	return 0;
        } else {
        	return 1;
        }
    } catch (const std::runtime_error& e) {
    	std::cerr << e.what() << std::endl;
        return 1;
    }
}
