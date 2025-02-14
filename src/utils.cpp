/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbecker <mbecker@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:30:39 by mbecker           #+#    #+#             */
/*   Updated: 2025/02/13 18:01:36 by mbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.h"

string itostr(int num) 
{
	ostringstream oss;
	oss << num;
	return oss.str();
}

int	countOccurrences(const string s, char c)
{
	int	count = 0;

	for (size_t i = 0; i < s.size(); i++)
		if (s.at(i) == c)
			count++;

	return (count);
}
