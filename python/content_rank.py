def content_rank(content_list, *args, **kwargs):

    if not kwargs.has_key('clear'):
        kwargs['clear'] = True
    if not kwargs.has_key('keys'):
        kwargs['keys'] = None
    
    rank = [0 for content in content_list]
    
    for i,content in enumerate(content_list):
        for kw in args:
            if kwargs['keys'] is not None:
                # content is a dict
                for key in kwargs['keys']:
                    if kw in content.get(key, ''):
                        rank[i] += 1
            else:
                # content is just a string
                if kw in content:
                    rank[i] += 1
    sorted_list = sorted(zip(rank, content_list), key=lambda item: item[0], reverse=True)
    
    if kwargs['clear']:
        return [item[1] for item in sorted_list if item[0]>0]
    else:
        return [item[1] for item in sorted_list]

