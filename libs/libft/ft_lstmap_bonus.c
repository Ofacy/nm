/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcottet <lcottet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:10:08 by lcottet           #+#    #+#             */
/*   Updated: 2023/11/09 12:23:39 by lcottet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*error_clear(t_list	*lst, void (*del)(void *), void *fcontent)
{
	ft_lstclear(&lst, del);
	if (fcontent)
		del(fcontent);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*mylistbegin;
	t_list	*myelement;
	t_list	*prevelement;
	void	*fcontent;

	myelement = NULL;
	mylistbegin = NULL;
	while (lst)
	{
		prevelement = myelement;
		fcontent = f(lst->content);
		if (!fcontent)
			return (error_clear(mylistbegin, del, NULL));
		myelement = ft_lstnew(fcontent);
		if (!myelement)
			return (error_clear(mylistbegin, del, fcontent));
		if (prevelement)
			prevelement->next = myelement;
		if (!mylistbegin)
			mylistbegin = myelement;
		lst = lst->next;
	}
	return (mylistbegin);
}
