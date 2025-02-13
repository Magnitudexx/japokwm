japokwm-container(5)

# Type
	class Container
# Description
	Represents a window.
# Static Variables
	container focused = focused container
		returns the focused container
# Static Methods
# Methods
	void focus()
		Focuses this container.
	void kill()
		Kills this container.
	void toggle_add_sticky(BitSet)
		xors the bitset at $1 with the bitset of this container's sticky state.
	void toggle_add_sticky_restricted(BitSet)
		xors the bitset at $1 with the bitset of this container's sticky state.
		Restricts that the state of the bit that represents the current
		tag is changed.
# Variables
	float alpha = 1.0
		The alpha value of this container.
	float ratio = 1.0
		The ratio of this container. If it is 0, the container is shown normally.
	bool sticky = BitSet(current_tag)
		If true, this container is sticky.
	bool sticky_restricted = BitSet(current_tag)
		If true, this container is sticky.
		Restricts that the state of the bit that represents the current
		tag is changed.
	int tag = current_tag
		The tag this container is on.
