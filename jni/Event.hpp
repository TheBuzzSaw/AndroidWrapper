#ifndef XpgEventHpp
#define XpgEventHpp

namespace XPG
{
	class Window;

	struct Ammo
	{
		Window* window;
		void* userData;
	};

	typedef void (*Target)(const Ammo&);

	class Event
	{
		public:
			Event();
			~Event();

			void Aim(Target target, void* userData = 0);
			void Fire(Ammo& ammo) const;

		private:
			Event(const Event& other)
			{
				// Nope!
			}

			Event& operator=(const Event& other)
			{
				// Nope!
				return *this;
			}

			Target _target;
			void* _userData;
	};
}

#endif
