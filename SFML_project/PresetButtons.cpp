#include "PresetButtons.h"
using namespace sf;

extern RenderWindow mainWindow;//
extern View view;//
extern MODES Mousemode;//
extern Menu mainMenu;

std::list<Object*> Drawer::allVisibleObjects;

Button moveButton = Button(&mainWindow,
	"Textures\\SFML_project\\MoveButton.jpg", MODE_MOVE, []()->Object* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f startPosition = view.getCenter();
		Vector2f startMousePosition(Vector2f(Mouse::getPosition(mainWindow)));
		Point* point = find.nearbyConstructedPoint(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;
			Vector2f delta = Vector2f(Mouse::getPosition(mainWindow)) - startMousePosition;
			Vector2f Scale;
			Scale.x = view.getSize().x / mainWindow.getSize().x;
			Scale.y = view.getSize().y / mainWindow.getSize().y;
			delta.x *= Scale.x;
			delta.y *= Scale.y;
			if (point)
			{
				point->moveTo(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
			}
			else
			{
				view.setCenter(startPosition - delta);
			}
			wait.sleep();
		}
		return nullptr;
	});

Button pointButton = Button(&mainWindow,
	"Textures\\SFML_project\\Point.png",
	MODE_POINT, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (point)
		{
			return nullptr;
		}
		point = find.nearbyNewPoint(mousePosition);
		return point;
	});

Button lineButton = Button(&mainWindow,
	"Textures\\SFML_project\\Line.png",
	MODE_LINE, []()->Object* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Line* line;
		if (points.first->isOnCircle() && points.second->isOnCircle())
		{
			line = new Chord(dynamic_cast<UnitPoint*>(points.first), dynamic_cast<UnitPoint*>(points.second));
		}
		else
		{
			line = new Line(points.first, points.second);
		}
		return line;
	});

Button pointBetweenPoints = Button(&mainWindow,
	"Textures\\SFML_project\\PointBetweenPoints.png",
	MODE_POINT_BETWEEN_POINTS, []()->Object* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Line* line;
		if (points.first->isOnCircle() && points.second->isOnCircle())
		{
			line = new Chord(dynamic_cast<UnitPoint*>(points.first), dynamic_cast<UnitPoint*>(points.second));
		}
		else
		{
			line = new Line(points.first, points.second);
		}
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);

		Point* point = new Point(line, points.first, points.second, mousePosition);
		return point;
	});

Button centralProjectionButton = Button(&mainWindow,
	"Textures\\SFML_project\\CentralProjection.png",
	MODE_CENTRAL_PROJECTION, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* firstPoint = find.nearbyConstructedPoint(mousePosition);
		if (!firstPoint)
		{
			firstPoint = find.nearbyNewPoint(mousePosition);
		}
		UnitPoint* secondPoint = nullptr;
		while (!secondPoint)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			secondPoint = find.nearbyConstructedPointOnCircle(mousePosition);
			if (!secondPoint)
			{
				secondPoint = find.nearbyNewPointOnCircle(mousePosition);
			}
		}
		Point* point = new UnitPoint(UnitCircle::getInstance(), firstPoint, secondPoint);
		return point;
	});

Button perpendicularButton = Button(&mainWindow,
	"Textures\\SFML_project\\Perpendicular.png",
	MODE_PERPENDICULAR, []()->Object* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		if (dynamic_cast<UnitPoint*>(point) && dynamic_cast<Chord*>(line))
		{
			UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(point), dynamic_cast<Chord*>(line));
			return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
		}
		Point* projPoint = new Point(point, line);
		return new Line(point, projPoint);
	});

Button midPointButton = Button(&mainWindow,
	"Textures\\SFML_project\\Midpoint.png",
	MODE_MIDPOINT, []()->Object* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Point* point = new Point(points.first, points.second, 1);
		return point;
	});

Button symmetryButton = Button(&mainWindow,
	"Textures\\SFML_project\\Symmetry.png",
	MODE_SYMMETRY, []()->Object* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Point* point = new Point(points.first, points.second, (float)-2);
		return point;
	});

Button rotateLeftButton = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_ROTATION_LEFT, []()->Object* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Point* point = new Point(points.first, points.second, -1);
		return point;
	});

Button rotateRightButton = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_ROTATION_RIGHT, []()->Object* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		Point* point = new Point(points.first, points.second, 1);
		return point;
	});

Button projectionButton = Button(&mainWindow,
	"Textures\\SFML_project\\Projection.png",
	MODE_PROJECTION, []()->Object* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		return new Point(point, line);
	});

Button parallelButton = Button(&mainWindow,
	"Textures\\SFML_project\\Parallel.png",
	MODE_PARALLEL, []()->Object* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
		}
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		if (dynamic_cast<Chord*>(line) && dynamic_cast<UnitPoint*>(point))
		{
			UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<Chord*>(line), dynamic_cast<UnitPoint*>(point));
			return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
		}
		return new Line(line, point);
	});

Button scalarButton = Button(&mainWindow,
	"Textures\\SFML_project\\Scalar.png",
	MODE_MIDPOINT, []()->Object* {
		InterruptionChecker checker;
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		ScalarBox* dialogBox = new ScalarBox(&mainWindow);
		while (!dialogBox->isFinished())
		{
			if (!checker.checkInterruption())
			{
				return nullptr;
			}
			wait.sleep();
		}
		float ratio = dialogBox->getDouble();
		delete dialogBox;
		new Point(points.first, points.second, ratio);
		return nullptr;
	});

Button tangentButton = Button(&mainWindow,
	"Textures\\SFML_project\\TangentButton.jpg",
	MODE_TANGENT, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPointOnCircle(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPointOnCircle(mousePosition);
		}
		if (point)
		{
			auto unitCircle = UnitCircle::getInstance();
			new Line(unitCircle, point);
		}
		return nullptr;
	});

Button deleteButton = Button(&mainWindow,
	"Textures\\SFML_project\\DeleteButton.jpg",
	MODE_DELETE, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyVisibleObject(mousePosition);
		if (object)
		{
			delete object;
			object = nullptr;
		}
		return nullptr;
	});

Button hideButton = Button(&mainWindow,
	"Textures\\SFML_project\\HideButton.jpg",
	MODE_HIDE, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyObject(mousePosition);
		if (object)
		{
			object->changeVisibility();
		}
		return nullptr;
	});

Button clearButton = Button(&mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg",
	MODE_CLEAR, []()->Object* {
		Waiter wait;
		while (Drawer::allVisibleObjects.size() > 1)
		{
			auto object = *prev(Drawer::allVisibleObjects.end());
			delete object;
			object = nullptr;
		}
		wait.sleep();
		return nullptr;
	});

Button switchButton = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_SWITCH, []()->Object* {
		mainMenu.switchLayer();
		return nullptr;
	});

Button fourPointsOnACircle = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_FOUR_POINTS, []()->Object* {
		Waiter wait;
		Finder find;
		const int fourTimes = 4;
		std::vector<Point*> points;
		for (int i = 0; i < fourTimes; ++i)
		{
			Point* point = nullptr;
			while (!point)
			{
				if (wait.untilClick())
				{
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}
		new Circle(points[0], points[1], points[2], points[3]);
		return nullptr;
	});

Button ñollinearityOfLines = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_THREE_LINES, []()->Object* {
		Waiter wait;
		Finder find;
		const int threeTimes = 3;
		std::vector<Line*> lines;
		for (int i = 0; i < threeTimes; ++i)
		{
			Line* line = nullptr;
			while (!line)
			{
				if (wait.untilClick())
				{
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				line = find.nearbyLine(mousePosition);
			}
			lines.push_back(line);
		}
		return nullptr;
	});

Button ñollinearityOfPoints = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_THREE_POINTS, []()->Object* {
		Waiter wait;
		Finder find;
		const int threeTimes = 4;
		std::vector<Point*> points;
		for (int i = 0; i < threeTimes; ++i)
		{
			Point* point = nullptr;
			while (!point)
			{
				if (wait.untilClick())
				{
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}
		return nullptr;
	});

Button proveConstructionButton = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_PROVE_CONSTRUCTION, []()->Object* {
		Waiter wait;
		Finder find;
		InterruptionChecker checker;
		Object* firstObject = nullptr;
		while (!firstObject)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			firstObject = find.nearbyVisibleObject(mousePosition);
		}
		mainMenu.switchLayer();
		Object* secondObject = nullptr;
		while (checker.checkInterruption())
		{
			wait.sleep();
		}
		while (!secondObject)
		{
			if (Mousemode == MODE_NOTHING || Mousemode == MODE_PROVE_CONSTRUCTION)
			{
				return nullptr;
			}
			secondObject = Creation::getInstance()->CurrentMethod();
		}
		wait.sleep();
	});

Button debugButton = Button(&mainWindow,
	"Textures\\SFML_project\\Test.jpg",
	MODE_PROVE_CONSTRUCTION, []()->Object* {
		Waiter wait;
		Finder find;
		InterruptionChecker checker;
		Point* point = nullptr;
		while (!point)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			point = find.nearbyConstructedPoint(mousePosition);
		}
		point->printExpr();
	});

Object* Finder::nearbyVisibleObject(Vector2f mousePosition)
{
	Point* point = nearbyConstructedPoint(mousePosition);
	if (point)
	{
		return point;
	}
	Line* line = nearbyLine(mousePosition);
	if (line)
	{
		return line;
	}
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (object && object->isNearby(mousePosition) && object->getVisibility())
		{
			return object;
		}
	}
	return nullptr;
}

Object* Finder::nearbyObject(Vector2f mousePosition)
{
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (object && object->isNearby(mousePosition))
		{
			return object;
		}
	}
	if (UnitCircle::getInstance()->isNearby(mousePosition))
	{
		return UnitCircle::getInstance();
	}
	return nullptr;
}

Point* Finder::nearbyConstructedPoint(Vector2f mousePosition)
{
	for (Object* object : Drawer::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point && point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
		}
	}
	return nullptr;
}

UnitPoint* Finder::nearbyConstructedPointOnCircle(Vector2f mousePosition)
{
	for (Object* object : Drawer::allVisibleObjects)
	{
		UnitPoint* point = dynamic_cast<UnitPoint*>(object);
		if (point && point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
		}
	}
	return nullptr;
}

Point* Finder::nearbyNewPoint(Vector2f mousePosition)
{
	Point* point = nearbyIntersection(mousePosition);
	if (point)
	{
		return point;
	}
	Line* line = nearbyLine(mousePosition);
	if (line)
	{
		//return new Point(line, mousePosition);
	}
	UnitCircle* unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new UnitPoint(unitCircle, mousePosition);
	}
	return new Point(mousePosition);
}

UnitPoint* Finder::nearbyNewPointOnCircle(Vector2f mousePosition)
{
	auto unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new UnitPoint(unitCircle, mousePosition);
	}
	return nullptr;
}

Line* Finder::nearbyLine(Vector2f mousePosition)
{
	for (Object* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line && line->isNearby(mousePosition) && line->getVisibility())
		{
			return line;
		}
	}
	return nullptr;
}

std::pair<Line*, Line*> Finder::nearbyLines(Vector2f mousePosition)
{
	std::pair<Line*, Line*> lines = { nullptr,nullptr };
	for (Object* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition) && line->getVisibility())
			{
				(std::get<0>(lines) ? lines.second : lines.first) = line;
				if (std::get<1>(lines)) return lines;
			}
		}
	}
	return { nullptr,nullptr };
}



Point* Finder::nearbyIntersection(Vector2f mousePosition)
{
	std::pair<Line*, Line*> lines = nearbyLines(mousePosition);
	if (lines.first)
	{
		Point* point = new Point(lines.first, lines.second);
		if (point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
		}
		else
		{
			delete point;
			point = nullptr;
		}
	}
	return nullptr;
}

UnitCircle* Finder::nearbyUnitCircle(Vector2f mousePosition)
{
	return nullptr;
}

InterruptionChecker::InterruptionChecker()
{
	checker.checkMode();
}

bool InterruptionChecker::checkInterruption()
{
	return checker.checkMode();
}

bool Waiter::mouseOnTheScreen()
{
	Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
	Vector2f center = view.getCenter();
	Vector2f size = view.getSize();
	Vector2f leftUpCorner = center - size / 2.f;
	Vector2f rightDownCorner = (center + size / 2.f);
	rightDownCorner.y += size.y * (mainWindowRect.height - mainWindowRect.top) - size.y;
	return mousePosition.x > leftUpCorner.x && mousePosition.y > leftUpCorner.y &&
		mousePosition.x < rightDownCorner.x&& mousePosition.y < rightDownCorner.y;
}

void Waiter::sleep()
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(sleepingTime));
}

bool Waiter::untilClick()
{
	while (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		sleep();
	}
	while (!Mouse::isButtonPressed(Mouse::Button::Left) || mainMenu.mouseOnMenu() || !mouseOnTheScreen())
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		sleep();
	}
	return false;
}
