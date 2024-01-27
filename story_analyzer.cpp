#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;


vector<vector<string>> Fantasy_data;
vector<vector<string>> Mystery_data;
vector<vector<string>> Romance_data;
vector<vector<string>> SciFi_data;

vector<vector<string>> stories_data;
vector<string> imported_stories;

vector<string> analyzed_stories;


int story_index = 0 ;

int Fantasy_probability;
int Mystery_probability;
int Romance_probability;
int SciFi_probability;



void get_input(vector<string> & tokens)
{
    string input;
    getline(cin, input);
    istringstream iss(input);
    string token;
    while (getline(iss, token, ' '))
    {
        tokens.push_back(token);
    }
}

void read_csv(const string& filename, vector<vector<string>>& data)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        getline(file, line);
        while (getline(file, line)) 
        {
            vector<string> row;
            stringstream ss(line);
            string cell;
            while (getline(ss, cell, ',')) 
            {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();
    }
    else
    {
        std::cout << "Unable to open file " << filename << endl;
    }
}

void import_story(const string& filename)
{
    ifstream file(filename);
    if (file.good())
    {
        if (file.is_open())
        {
            string line;
            vector<string> temp;
            while (getline(file, line, ' '))
            {
                temp.push_back(line);
            }
            stories_data.push_back(temp);
            string name = filename.substr(0, filename.find_last_of('.'));
            imported_stories.push_back(name);
            story_index += 1;
            std::cout << name << " imported successfully."<<endl;
            file.close();
        }
        else if (!file.is_open())
        {
            std::cout << "Error importing the story." << endl;
        }
    }
    else 
    {
        std::cout << "File not found." << endl;
    }
}

void show_the_list_of_commands()
{
    std::cout << "show_the_list_of_commands" << endl;
    std::cout << "import_story" << endl;
    std::cout << "show_the_list_of_stories" << endl;
    std::cout << "analyze_story" << endl;
    std::cout << "analyzed_stories_list" << endl;
    std::cout << "show_story_analysis" << endl;
    std::cout << "dump_analyzed_stories" << endl;
    std::cout << "exit" << endl;
}

void show_story_analysis(string index)
{

}

void analyzed_stories_list()
{
    string operat = ",";
    cout << "The analyzed stories are:";
    int analized_size = analyzed_stories.size();
    for (size_t i = 0; i < analized_size; i++)
    {
        if (i == analized_size - 1)
        {
            operat = "and";
        }
        cout <<" " << analyzed_stories[i] + operat;
    }
    cout << ".";
}

void dump_analyzed_stories(string filename)
{

}

void read_files()
{
    read_csv("Fantasy.csv", Fantasy_data);
    read_csv("Mystery.csv", Mystery_data);
    read_csv("Romance.csv", Romance_data);
    read_csv("SciFi.csv", SciFi_data);
}

void show_the_list_of_stories()
{
    int i = 1;
    for (const auto& row : imported_stories)
    {
        std::cout << i << ". " << row << endl;
        i += 1;
    }
}

void analyzer(int index , string filename , vector<vector<string>> Genre_data, vector<vector<string>> Genre_temp_data , vector<string> Genre_common_words , int& Genre_probability)
{
    for (int i = 0; i < Genre_data.size(); i++)
    {
        vector<string> Genre_temp_data_row;
        Genre_temp_data_row.push_back(Genre_data[i][0]);
        Genre_temp_data_row.push_back(Genre_data[i][1]);
        Genre_temp_data_row.push_back("0");
        Genre_temp_data.push_back(Genre_temp_data_row);
    }
    for (int i = 0; i < stories_data[index - 1].size(); i++)
    {
        for (int j = 0; j < Genre_temp_data.size();j++)
        {
            if (stories_data[index - 1][i] == Genre_temp_data[j][0])
            {
                Genre_temp_data[j][2] = to_string(stoi(Genre_temp_data[j][2]) + 1);
            }
        }
    }
    for (int j = 0; j < Genre_temp_data.size(); j++)
    {
        Genre_probability += (stoi(Genre_temp_data[j][1]) * stoi(Genre_temp_data[j][2]));
    }

    int A, B, T;
    for (int i = 0; i < Genre_temp_data.size()-1; i++)
    {
        A = stoi(Genre_temp_data[i][2]);
        B = stoi(Genre_temp_data[i + 1][2]);
        if (B > A)
        {
            T = A;
            A = B;
            B = T;
            Genre_temp_data[i][2] = to_string(A);
            Genre_temp_data[i + 1][2] = to_string(B);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        cout << Genre_temp_data[i][2];
        Genre_common_words.push_back(Genre_temp_data[i][2]);
    }
}

void analyze_story(int index, string filename)
{
    analyzed_stories.push_back(imported_stories[index - 1]);
    vector<string> Fantasy_common_words;
    vector<vector<string>> Fantasy_temp_data;
    analyzer(index, filename, Fantasy_data, Fantasy_temp_data, Fantasy_common_words, Fantasy_probability);

    vector<string> Romance_common_words;
    vector<vector<string>> Romance_temp_data;
    analyzer(index, filename, Romance_data, Romance_temp_data, Romance_common_words, Romance_probability);

    vector<string> Mystery_common_words;
    vector<vector<string>> Mystery_temp_data;
    analyzer(index, filename, Fantasy_data, Mystery_temp_data, Mystery_common_words, Mystery_probability);

    vector<string> SciFi_common_words;
    vector<vector<string>> SciFi_temp_data;
    analyzer(index, filename, SciFi_data, SciFi_temp_data, SciFi_common_words, SciFi_probability);

    fstream output_file(filename, ios::out);
    int arr[] = { Fantasy_probability,Mystery_probability,Romance_probability,SciFi_probability };
    int  k = Fantasy_probability + Mystery_probability + Romance_probability + SciFi_probability;
    std::sort(arr, arr + 4);
    if (arr[3] == Fantasy_probability)
    {
        std::cout << "The genre of the story " << imported_stories[index - 1] << " is " << "Fantasy"<<endl;
        output_file << "Story Name: " << imported_stories[index - 1] << endl;
        output_file << "Predicted Genre: Fantasy\n" << endl;
        output_file << "Genre,Number of keywords,Confidence" << endl;
        output_file << "Fantasy," << Fantasy_temp_data.size()<< Fantasy_probability << float(Fantasy_probability) / float(k) << endl;
        output_file << "Mystery," << Mystery_temp_data.size()<< Mystery_probability << float(Mystery_probability) / float(k) << endl;
        output_file << "Romance," << Romance_temp_data.size()<< Romance_probability << float(Romance_probability) / float(k) << endl;
        output_file << "SciFi,"   << SciFi_temp_data.size() << SciFi_probability  << float(SciFi_probability) / float(k) << endl;
        output_file << "the common keywords of the story are: ";
        for (int i = 0; i < Fantasy_common_words.size(); i++)
        {
            output_file << Fantasy_common_words[i];
        }
        output_file << '.' << endl;
    }
    else if (arr[3] == Mystery_probability)
    {
        std::cout << "The genre of the story " << imported_stories[index - 1] << " is " << "Mystery" << endl;
        output_file << "Story Name: " << imported_stories[index - 1] << endl;
        output_file << "Predicted Genre: Mystery\n" << endl;
        output_file << "Genre,Number of keywords,Confidence" << endl;
        output_file << "Mystery," << Mystery_temp_data.size()<< Mystery_probability << float(Mystery_probability) / float(k) << endl;
        output_file << "Fantasy," << Fantasy_temp_data.size() << Fantasy_probability << float(Fantasy_probability) / float(k) << endl;
        output_file << "Romance," << Romance_temp_data.size()<< Romance_probability << float(Romance_probability) / float(k) << endl;
        output_file << "SciFi,"   << SciFi_temp_data.size() << SciFi_probability << float(SciFi_probability) / float(k) << endl;
        output_file << "the common keywords of the story are: ";
        for (int i = 0; i < Mystery_common_words.size(); i++)
        {
            output_file << Mystery_common_words[i];
        }
        output_file << '.' << endl;
    }
    else if (arr[3] == Romance_probability)
    {
        std::cout << "The genre of the story " << imported_stories[index - 1] << " is " << "Romance" << endl;
        output_file << "Story Name: " << imported_stories[index - 1] << endl;
        output_file << "Predicted Genre: Romance\n" << endl;
        output_file << "Genre,Number of keywords,Confidence" << endl;
        output_file << "Romance," << Romance_temp_data.size()<< Romance_probability << float(Romance_probability) / float(k) << endl;
        output_file << "Mystery," << Mystery_temp_data.size() << Mystery_probability << float(Mystery_probability) / float(k) << endl;
        output_file << "Fantasy," << Fantasy_temp_data.size() << Fantasy_probability<<  float(Fantasy_probability) / float(k) << endl;
        output_file << "SciFi,"     << SciFi_temp_data.size() << SciFi_probability << float(SciFi_probability) / float(k) << endl;
        output_file << "the common keywords of the story are: ";
        for (int i = 0; i < Romance_common_words.size(); i++)
        {
            output_file << Romance_common_words[i];
        }
        output_file << '.' << endl;
    }
    else if(arr[3] == SciFi_probability)
    {
        std::cout << "The genre of the story " << imported_stories[index - 1] << " is " << "SciFi" << endl;
        output_file << "Story Name: " << imported_stories[index - 1] << endl;
        output_file << "Predicted Genre: SciFi\n" << endl;
        output_file << "Genre,Number of keywords,Confidence" << endl;
        output_file << "SciFi,"   << SciFi_temp_data.size()<< SciFi_probability << float(SciFi_probability) / float(k) << endl;
        output_file << "Romance," << Romance_temp_data.size()<< Romance_probability << float(Romance_probability) / float(k) << endl;
        output_file << "Mystery," << Mystery_temp_data.size()<< Mystery_probability << float(Mystery_probability) / float(k) << endl;
        output_file << "Fantasy," << Fantasy_temp_data.size()<< Fantasy_probability << float(Fantasy_probability) / float(k) << endl;
        output_file << "the common keywords of the story are: ";
        for (int i = 0; i < SciFi_common_words.size(); i++)
        {
            output_file << SciFi_common_words[i];
        }
        output_file << '.' << endl;
    }
    output_file.close();
}

int main()
{
    read_files();

    while (true)
    {
        vector<string> inputs;
        get_input(inputs);
        if (inputs[0] == "show_the_list_of_commands")show_the_list_of_commands();
        else if (inputs[0] == "import_story")
        {
            if (inputs.size() != 1)
            {
                import_story(inputs[1]);
            }
            else
            {
                std::cout << "import_story {filename.txt}" << endl;
            }
        }
        else if (inputs[0] == "show_the_list_of_stories") show_the_list_of_stories();
        else if (inputs[0] == "analyze_story")
        {
            if (inputs.size() == 3)
            {
                if (story_index == 0)
                {
                    std::cout << "no story found." << endl;
                    continue;
                }
                int index = stoi(inputs[1]);
                {
                    if ((index > story_index) || (index < 1))
                    {
                        std::cout << "Invalid story index." << endl;
                    }
                    else
                    {
                        analyze_story(index, inputs[2]);
                    }
                }
            }
            else
            {
                std::cout << "analyze_story {story_index} {output_file_name.txt}" << endl;
            }
        }
        else if (inputs[0] == "show_story_analysis")
        {
            if (inputs.size() == 2)
            {
                show_story_analysis(inputs[1]);
            }
            else
            {
                std::cout << "show_story_analysis { story_index }" << endl;
            }
        }
        else if (inputs[0] == "dump_analyzed_stories")
        {
            if (inputs.size() == 2)
            {
                dump_analyzed_stories(inputs[1]);
            }
            else
            {
                std::cout << "dump_analyzed_stories {output_file_name.csv}" << endl;
            }
        }
        else if (inputs[0] == "analyzed_stories_list") analyzed_stories_list();
        else if (inputs[0] == "exit")break;
        else std::cout << "Command not found. see the list of commands with show_the_list_of_commands."<<endl;
    }
    return 0;
}







//fstream csv_write("test.csv" , ios::out);
//csv_write << "anoosh,vahid,kingdom\n";
//csv_write << "45,32,12";
//csv_write.close();


//fstream csv_read("Brushstrokes_And_Sunsets.txt", ios::in);
//if (!csv_read.is_open()) std::cout << "file not open";
//string line;
//while (getline(csv_read, line , ' '))
//{
//    std::cout << line << endl;
//}