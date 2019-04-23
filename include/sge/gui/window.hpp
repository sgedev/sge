//
//
#ifndef SGE_GUI_WINDOW_HPP
#define SGE_GUI_WINDOW_HPP

#include <string>

#include <sge/noncopyable.hpp>
#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class window: public noncopyable {
public:
	window(void);
	virtual ~window(void);

public:
	virtual bool init(window *parent, const char *name, int x, int y, int width, int height, int flags = 0);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(void);
	window *get_parent(void);
	window *first_child(void);
	window *next_sibling(void);

private:
	class list final: public noncopyable {
	public:
		class node final: public noncopyable {
		public:
			node(void);
			~node(void);

		public:
			void reset(void);
			void link(node *prev, node *next);
			void unlink(void);
			bool is_linked(void) const;
			node *get_prev(void);
			node *get_next(void);

		private:
			node *m_prev;
			node *m_next;
		};

	public:
		list(void);
		~list(void);

	public:
		void reset(void);
		bool is_empty(void) const;
		size_t get_length(void) const;
		node *get_knot(void);
		const node *get_knot(void) const;
		node *get_head(void);
		const node *get_head(void) const;
		node *get_tail(void);
		const node *get_tail(void) const;
		void add_head(node *p);
		void add_tail(node *p);
		node *remove_head(void);
		node *remove_tail(void);
		void remove(node *p);
		bool has(const node *p) const;

	private:
		node m_knot;
		size_t m_length;
	};

private:
	window *m_parent;
	node m_node;
	node m_children;
	glm::ivec4 m_rect;
	int m_flags;
	std::string m_name;
};

SGE_GUI_END

#endif // SGE_GUI_WINDOW_HPP

