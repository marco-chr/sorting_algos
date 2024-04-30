// COMP4300 - assignment 1
//  This program read description from config text file
// name, position, speed, color, properties specific to shape
// bouncing off the window sides

// font size and color from file to define shape name in the center of the shape
// list all the shapes. shapes are selectable to edit their properties
// draw toggle
// scale
// x,y velocity
// color
// name

// types of line (window, font, rectangle, circle)

// std::vector container to store number of shapes
// custom class to store prop of a shape


#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <chrono>
#include <thread>



// class Shape - shape is a generic graphics element to be displayed

typedef struct rect_data {
	int r_height;
	int color1;
	int color2;
	int color3;
};

void generate_vec(std::vector<rect_data>& m_rect_data_vec) {

	m_rect_data_vec.clear();
	for (int i = 1; i < 100; i++) {

		rect_data m_rdata;

		m_rdata.r_height = rand() % 600;
		m_rdata.color1 = rand() % 255;
		m_rdata.color2 = rand() % 255;
		m_rdata.color3 = rand() % 255;
		m_rect_data_vec.push_back(m_rdata);

	}
}

void generate_rects(std::vector<sf::RectangleShape>& m_rects, std::vector<rect_data>& m_rect_data_vec, sf::RenderWindow& m_window) {

	m_rects.clear();
	m_window.clear();
	for (int i = 1; i < 100; i++) {
		sf::RectangleShape rect(sf::Vector2f(10, m_rect_data_vec[i].r_height));
		rect.setFillColor(sf::Color(m_rect_data_vec[i].color1, m_rect_data_vec[i].color2, m_rect_data_vec[i].color3));
		rect.setPosition(sf::Vector2f(100 + i * 10, 100));
		m_rects.push_back(rect);
	}

	for (auto e : m_rects)
		m_window.draw(e);

	m_window.display();
}

int binarySearch(std::vector<rect_data>& a, rect_data item, int low, int high) {
	if (high <= low)
		return (item.r_height > a[low].r_height) ? (low + 1) : low;

	int mid = (low + high) / 2;

	if (item.r_height == a[mid].r_height)
		return mid + 1;
	if (item.r_height >= a[mid].r_height)
		return binarySearch(a, item, mid + 1, high);

	return binarySearch(a, item, low, mid - 1);
}

void insertionSort(std::vector<rect_data>& inputvec, std::vector<sf::RectangleShape>& m_rects, sf::RenderWindow& m_window, int n)
{
	int i, loc, j, k;
	rect_data selected;

	for (i = 1; i < n; ++i)
	{
		j = i - 1;
		selected = inputvec[i];

		loc = binarySearch(inputvec, selected, 0, j);

		while (j >= loc)
		{
			inputvec[j + 1] = inputvec[j];
			j--;
		}
		inputvec[j + 1] = selected;
		generate_rects(m_rects, inputvec, m_window);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void bubbleSort(std::vector<rect_data>& inputvec, std::vector<sf::RectangleShape>& m_rects, sf::RenderWindow& m_window, int size) {

	// loop to access each array element
	for (int step = 0; step < size; ++step) {

		// loop to compare array elements
		for (int i = 0; i < size - step; ++i) {

			// compare two adjacent elements
			// change > to < to sort in descending order
			if (inputvec[i].r_height > inputvec[i + 1].r_height) {

				// swapping elements if elements
				// are not in the intended order
				rect_data temp = inputvec[i];
				inputvec[i] = inputvec[i + 1];
				inputvec[i + 1] = temp;
				generate_rects(m_rects, inputvec, m_window);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}
	}
}


int main(int argc, char* argv[])
{
	std::vector<sf::RectangleShape> rects;
	rect_data rdata;
	std::vector<rect_data> rect_data_vec;

	int wWidth = 1280, wHeight = 720;

	// setup font
	sf::Font myFont;
	if (!myFont.loadFromFile("fonts/tech.ttf"))
	{
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML TEST");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	sf::Clock deltaClock;
	srand(time(NULL));

	generate_vec(rect_data_vec);
	generate_rects(rects, rect_data_vec, window);

	while (window.isOpen())
	{
		// event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// key is pressed
			if (event.type == sf::Event::KeyPressed)
			{
				// key pressed to console
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				// when x is pressed
				if (event.key.code == sf::Keyboard::X)
				{
					generate_vec(rect_data_vec);
					generate_rects(rects, rect_data_vec, window);
				}

				if (event.key.code == sf::Keyboard::O)
				{
					insertionSort(rect_data_vec, rects, window, 100);
				}

				if (event.key.code == sf::Keyboard::P)
				{
					bubbleSort(rect_data_vec, rects, window, 100);
				}
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		window.clear();

		for (auto e : rects)
		window.draw(e);
		
		// render window
		window.display();
	}

	return 0;

}
